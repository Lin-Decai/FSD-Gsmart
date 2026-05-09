import rclpy
from rclpy.node import Node
import sensor_msgs.msg as sensor_msgs
import std_msgs.msg as std_msgs
import sensor_msgs_py.point_cloud2 as pc2
import numpy as np
from scipy.spatial.transform import Rotation as R
import os

class TestLidarPublisher(Node):
    def __init__(self):
        super().__init__('test_lidar_publisher')
        self.declare_parameters('', [
            ('topic', '/perception/lidar_cluster'),
            ('publish_rate', 10.0),
            ('mode', 'identity'),
            ('transform.roll', 0.0),
            ('transform.pitch', 0.0),
            ('transform.yaw', 0.0),
            ('transform.translation_x', 0.0),
            ('transform.translation_y', 0.0),
            ('transform.translation_z', 0.0)
        ])
        topic = self.get_parameter('topic').value
        self.pub = self.create_publisher(sensor_msgs.PointCloud2, topic, 10)
        timer_period = 1.0 / self.get_parameter('publish_rate').value
        self.timer = self.create_timer(timer_period, self.publish_pointcloud)

        # 加载标准地图点云（从你的 skidpad.pcd 文件）
        self.map_points = self.load_map_points()
        self.mode = self.get_parameter('mode').value
        self.get_logger().info(f'Publisher ready, mode={self.mode}, {len(self.map_points)} points')

    def load_map_points(self):
        """直接硬编码读取 pcd，或这里简化，我们手动放置一些已知点来验证"""
        # 这里为了简单，我们生成模拟的锥桶坐标（与你的 ICP 内部 map 一致）
        # 实际你可以读取你真实的 PCD 文件，但注意依赖 pcl，这里我们用 numpy 构造示例
        # 以下坐标取自原 ICP 代码中的逻辑：x = point.y + start_length + lidar2imu, y = -point.x
        # 但更直接的方式：从你的 skidpad.pcd 提取，或在下文提供生成函数
        # 我们先用几个点示意，你可以替换成完整列表
        points = []
        # 左圆中心 (15+1.87, 9.125) 附近，角度从 0 到 2pi，半径 9.125
        start_len = 15.0
        lidar2imu = 1.87
        radius = 9.125
        # 生成内外圆点（极简版，仅作测试）
        for theta in np.linspace(0, 2*np.pi, 20):
            # 左圆 (逆时针)
            x = start_len + lidar2imu + radius * np.cos(theta)
            y = radius * np.sin(theta)
            points.append([x, y, 0.0])
        for theta in np.linspace(0, 2*np.pi, 20):
            # 右圆 (顺时针，但坐标形式同)
            x = start_len + lidar2imu + radius * np.cos(theta)
            y = -radius * np.sin(theta)
            points.append([x, y, 0.0])
        return np.array(points)

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
            rot = R.from_euler('xyz', [roll, pitch, yaw])
            rotated = rot.apply(points[:, :3])  # (N,3)
            translated = rotated + np.array([tx, ty, tz])
            return translated
        else:
            return points

    def publish_pointcloud(self):
        points = self.apply_transform(self.map_points)
        # 创建 PointCloud2 消息
        header = std_msgs.Header()
        header.stamp = self.get_clock().now().to_msg()
        header.frame_id = 'map'  # 或者与 ICP 坐标系一致

        # 构建点云数据
        fields = [
            sensor_msgs.PointField(name='x', offset=0, datatype=sensor_msgs.PointField.FLOAT32, count=1),
            sensor_msgs.PointField(name='y', offset=4, datatype=sensor_msgs.PointField.FLOAT32, count=1),
            sensor_msgs.PointField(name='z', offset=8, datatype=sensor_msgs.PointField.FLOAT32, count=1),
        ]
        cloud_data = []
        for pt in points:
            cloud_data.append([pt[0], pt[1], pt[2]])
        cloud_arr = np.array(cloud_data, dtype=np.float32)
        msg = pc2.create_cloud(header, fields, cloud_arr)
        self.pub.publish(msg)
        self.get_logger().debug(f'Published cloud with {len(cloud_arr)} points')

def main(args=None):
    rclpy.init(args=args)
    node = TestLidarPublisher()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()