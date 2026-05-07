import rclpy
from rclpy.node import Node
import numpy as np
import yaml
import os
from ament_index_python.packages import get_package_share_directory
from sensor_msgs.msg import PointCloud2, CameraInfo, Image, PointField
import sensor_msgs_py.point_cloud2 as pc2
import math
import cv2
from cv_bridge import CvBridge
from yolo_ros.msg import DetectionArray, DetectedObject, DetectedObjectArray
from geometry_msgs.msg import Point, Pose
import struct

class LidarToImageProjection(Node):
    def __init__(self):
        super().__init__('lidar_to_image_projection1')

        # 加载配置文件
        package_share_directory = get_package_share_directory('ros2_lidar_camera_fusion_with_detection')
        yaml_file_path = os.path.join(package_share_directory, 'config', 'setup_config1.yaml')
        yaml_data = self.load_yaml_file(yaml_file_path)
        self.transformation_matrix = np.array(yaml_data['transformation_matrix'])
        self.depth_range_min = yaml_data['depth_range']['min']
        self.depth_range_max = yaml_data['depth_range']['max']

        self.get_logger().info(f'Loaded transformation matrix:\n{self.transformation_matrix}')
        self.get_logger().info(f'Loaded depth_range: min={self.depth_range_min}, max={self.depth_range_max}')
        self.get_logger().info('Lidar & Camera Fusion Node has been started.')

        # 初始化变量
        self.current_image = None
        self.bridge = CvBridge()
        self.detections = []
        self.fx = self.fy = self.cx = self.cy = None  # 相机内参
        self.image_width = self.image_height = 0

        # 订阅器
        self.subscription = self.create_subscription(PointCloud2, '/hesai/roi_cloud', self.pointcloud_callback, 10)
        self.camera_info_subscription = self.create_subscription(CameraInfo, 'camera_1/camera_info_1', self.camera_info_callback, 10)
        self.image_subscription = self.create_subscription(Image, '/camera_1/image_raw_1', self.image_callback, 10)
        self.detection_subscription = self.create_subscription(DetectionArray, '/yolo_camera1/tracking', self.detection_callback, 10)

        # 发布器
        self.detected_object_array_publisher = self.create_publisher(DetectedObjectArray, '/detected_object_position1', 10)
        self.object_pointcloud_publisher = self.create_publisher(PointCloud2, '/detected_object_pointcloud1', 10)

    def load_yaml_file(self, yaml_file):
        with open(yaml_file, 'r') as file:
            return yaml.safe_load(file)

    def camera_info_callback(self, msg):
        self.fx, self.fy, self.cx, self.cy = msg.k[0], msg.k[4], msg.k[2], msg.k[5]

    def image_callback(self, msg):
        self.current_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')
        self.image_width, self.image_height = msg.width, msg.height

    def detection_callback(self, msg):
        self.detections = []
        for detection in msg.detections:
            x1 = detection.bbox.center.position.x - detection.bbox.size.x / 2
            y1 = detection.bbox.center.position.y - detection.bbox.size.y / 2
            x2 = detection.bbox.center.position.x + detection.bbox.size.x / 2
            y2 = detection.bbox.center.position.y + detection.bbox.size.y / 2
            self.detections.append({
                'bbox': (x1, y1, x2, y2),
                'class_id': detection.class_id,
                'class_name': detection.class_name,
                'score': detection.score
            })

    def pointcloud_callback(self, msg):
        if self.current_image is None or any(val is None for val in [self.fx, self.fy, self.cx, self.cy]):
            self.get_logger().warning("Camera intrinsics or current image not available.")
            return

        points = self.point_cloud_data_extraction(msg)
        
        bbox_values = {i: {'x': [], 'y': [], 'z': [], 'points': []} for i in range(len(self.detections))}

        for point in points:
            x, y, z = point
            transformed_point, u, v = self.transformation(x, y, z, self.transformation_matrix)

            # 仅处理在图像范围内的点
            if 0 <= u < self.image_width and 0 <= v < self.image_height:
                for i, detection in enumerate(self.detections):
                    x1, y1, x2, y2 = detection['bbox']
                    if x1 <= u <= x2 and y1 <= v <= y2:
                        bbox_values[i]['x'].append(transformed_point[0])
                        bbox_values[i]['y'].append(transformed_point[1])
                        bbox_values[i]['z'].append(transformed_point[2])
                        bbox_values[i]['points'].append(point)

        # 发布检测目标信息
        self.detected_object_position(bbox_values, msg.header)
        self.publish_object_pointclouds(bbox_values, msg.header)

    def point_cloud_data_extraction(self, msg):
        points = []
        for point in pc2.read_points(msg, field_names=("x", "y", "z"), skip_nans=True):
            x, y, z = point[0], point[1], point[2]
            if math.isfinite(x) and math.isfinite(y) and math.isfinite(z) and self.depth_range_min < x < self.depth_range_max:
                points.append([x, y, z])
        return points

    def transformation(self, x, y, z, transformation_matrix):
        homogeneous_point = np.array([x, y, z, 1.0])
        transformed_point = np.dot(transformation_matrix, homogeneous_point)
        u = (self.fx * transformed_point[0] / transformed_point[2]) + self.cx
        v = (self.fy * transformed_point[1] / transformed_point[2]) + self.cy
        return transformed_point, u, v

    def publish_object_pointclouds(self, bbox_values, header):
        for i, values in bbox_values.items():
            if values['points']:
                obj_pc = self.create_pointcloud2_msg(values['points'], header)
                self.object_pointcloud_publisher.publish(obj_pc)

    def detected_object_position(self, bbox_values, header):
        detected_objects_msg = DetectedObjectArray()
        detected_objects_msg.header = header
        detected_objects_msg.header.frame_id = "camera1_link"
        for i, values in bbox_values.items():
            if values['points'] and i < len(self.detections):
                obj_msg = DetectedObject()
                obj_msg.class_id = self.detections[i]['class_id']
                obj_msg.class_name = self.detections[i]['class_name']
                obj_msg.pose.position.x = sum(values['x'])/len(values['x'])
                obj_msg.pose.position.y = sum(values['y'])/len(values['y'])
                obj_msg.pose.position.z = sum(values['z'])/len(values['z'])
                obj_msg.pose.orientation.w = 1.0
                detected_objects_msg.objects.append(obj_msg)
        self.detected_object_array_publisher.publish(detected_objects_msg)

    def create_pointcloud2_msg(self, points, header, with_color=False):
        fields = [
            PointField(name='x', offset=0, datatype=PointField.FLOAT32, count=1),
            PointField(name='y', offset=4, datatype=PointField.FLOAT32, count=1),
            PointField(name='z', offset=8, datatype=PointField.FLOAT32, count=1),
        ]
        if with_color:
            fields.append(PointField(name='rgb', offset=12, datatype=PointField.FLOAT32, count=1))
        
        point_data = b''
        for point in points:
            if with_color and len(point)>=6:  # [x,y,z,r,g,b]
                x,y,z,r,g,b = point[:6]
                rgb = struct.unpack('I', struct.pack('BBBB', b, g, r, 0))[0]
                point_data += struct.pack('ffff', x, y, z, rgb)
            else:
                x,y,z = point[:3]
                point_data += struct.pack('fff', x, y, z)
        
        return PointCloud2(
            header=header,
            height=1,
            width=len(points),
            is_dense=False,
            is_bigendian=False,
            fields=fields,
            point_step=16 if with_color else 12,
            row_step=(16 if with_color else 12)*len(points),
            data=point_data
        )

def main(args=None):
    rclpy.init(args=args)
    node = LidarToImageProjection()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

