import rclpy
from rclpy.node import Node
import sensor_msgs.msg as sensor_msgs
import std_msgs.msg as std_msgs
import sensor_msgs_py.point_cloud2 as pc2
import numpy as np
import os

def rotation_matrix(roll, pitch, yaw):
    cr, sr = np.cos(roll), np.sin(roll)
    cp, sp = np.cos(pitch), np.sin(pitch)
    cy, sy = np.cos(yaw), np.sin(yaw)
    R_x = np.array([[1, 0, 0], [0, cr, -sr], [0, sr, cr]])
    R_y = np.array([[cp, 0, sp], [0, 1, 0], [-sp, 0, cp]])
    R_z = np.array([[cy, -sy, 0], [sy, cy, 0], [0, 0, 1]])
    return R_z @ R_y @ R_x

class TestLidarPublisher(Node):
    def __init__(self):
        super().__init__('test_lidar_publisher')
        self.declare_parameters('', [
            ('topic', '/perception/lidar_cluster'),
            ('publish_rate', 10.0),
            ('mode', 'identity'),
            ('pcd_path', '/home/limdegcai/GSMART/CODE/FSD-Gsmart/planning_control/skidpad_ws/src/skidpad_icp/config/skidpad.pcd'),
            ('start_length', 15.0),
            ('lidar2imu', 1.87),
            ('transform.roll', 0.0),
            ('transform.pitch', 0.0),
            ('transform.yaw', 0.5236),
            ('transform.translation_x', 2.0),
            ('transform.translation_y', -1.5),
            ('transform.translation_z', 0.0)
        ])
        topic = self.get_parameter('topic').value
        self.pub = self.create_publisher(sensor_msgs.PointCloud2, topic, 10)
        timer_period = 1.0 / self.get_parameter('publish_rate').value
        self.timer = self.create_timer(timer_period, self.publish_pointcloud)

        self.map_points = self.load_map_points()
        self.mode = self.get_parameter('mode').value
        self.get_logger().info(f'Publisher ready, mode={self.mode}, {len(self.map_points)} points')

    def load_map_points(self):
        """读取 skidpad.pcd 并转换为与 ICP 模块相同的坐标系"""
        # 手动解析 PCD 文件（不依赖 pcl）
        pcd_path = self.get_parameter('pcd_path').value
        points_raw = []
        with open(pcd_path, 'r') as f:
            lines = f.readlines()
            data_start = False
            for line in lines:
                if line.startswith('DATA'):
                    data_start = True
                    continue
                if data_start:
                    parts = line.strip().split()
                    if len(parts) >= 3:
                        try:
                            x_pcd = float(parts[0])
                            y_pcd = float(parts[1])
                            points_raw.append([x_pcd, y_pcd])
                        except:
                            pass
        if not points_raw:
            self.get_logger().error('No points found in PCD file')
            return np.zeros((0, 3))

        start_len = self.get_parameter('start_length').value
        lidar2imu = self.get_parameter('lidar2imu').value
        # 与 skidpad_detector.cpp 完全一致的坐标变换
        map_pts = []
        for pt in points_raw:
            x_new = pt[1] + start_len + lidar2imu
            y_new = -pt[0]
            map_pts.append([x_new, y_new, 0.0])
        return np.array(map_pts)

    def apply_transform(self, points):
        mode = self.mode
        if mode == 'identity':
            return points
        elif mode == 'shifted':
            roll = self.get_parameter('transform.roll').value
            pitch = self.get_parameter('transform.pitch').value
            yaw = self.get_parameter('transform.yaw').value
            tx = self.get_parameter('transform.translation_x').value
            ty = self.get_parameter('transform.translation_y').value
            tz = self.get_parameter('transform.translation_z').value
            R_mat = rotation_matrix(roll, pitch, yaw)
            rotated = (R_mat @ points[:, :3].T).T
            translated = rotated + np.array([tx, ty, tz])
            return translated
        else:
            return points

    def publish_pointcloud(self):
        points = self.apply_transform(self.map_points)
        header = std_msgs.Header()
        header.stamp = self.get_clock().now().to_msg()
        header.frame_id = 'map'

        fields = [
            sensor_msgs.PointField(name='x', offset=0, datatype=sensor_msgs.PointField.FLOAT32, count=1),
            sensor_msgs.PointField(name='y', offset=4, datatype=sensor_msgs.PointField.FLOAT32, count=1),
            sensor_msgs.PointField(name='z', offset=8, datatype=sensor_msgs.PointField.FLOAT32, count=1),
        ]
        cloud_data = []
        for pt in points:
            cloud_data.append([pt[0], pt[1], pt[2]])
        cloud_arr = np.array(cloud_data, dtype=np.float32)
        # 添加调试打印
        self.get_logger().info(
            f'Publishing {len(cloud_arr)} points, first 3: '
            f'({cloud_arr[0,0]:.3f},{cloud_arr[0,1]:.3f}), '
            f'({cloud_arr[1,0]:.3f},{cloud_arr[1,1]:.3f}), '
            f'({cloud_arr[2,0]:.3f},{cloud_arr[2,1]:.3f})'
        )
        msg = pc2.create_cloud(header, fields, cloud_arr)
        self.pub.publish(msg)
        self.get_logger().debug(f'Published {len(cloud_arr)} points')

def main(args=None):
    rclpy.init(args=args)
    node = TestLidarPublisher()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()