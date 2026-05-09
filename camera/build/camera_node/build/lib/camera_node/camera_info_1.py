#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import CameraInfo
import numpy as np

class CameraInfoPublisher(Node):
    def __init__(self):
        # 1. 【必须首先调用】父类 Node 的构造函数，初始化节点
        # 参数是节点名称，需唯一
        super().__init__('camera_info_publisher_1')
        
        # 2. 创建 CameraInfo 发布者（话题名：camera_1/camera_info_1）
        self.camera_info_publisher = self.create_publisher(
            CameraInfo, 
            'camera_1/camera_info_1', 
            10  # QoS深度，队列最多缓存10条消息
        )
        
        # 3. 相机内参和畸变系数（您提供的参数）
        self.camera_matrix = np.array([[1926.06478016793, 0, 977.635390192986],
                                       [0, 1926.75557277439, 522.665951222063],
                                       [0, 0, 1]])
        self.dist_coeffs = np.array([-0.526848084119857, 0.279619545278389, 
                                     0.00362168285490537, 0.000267399425785593, 
                                     -0.0872414844157106])
        self.image_width = 1920  # 图像宽度（像素）
        self.image_height = 1080 # 图像高度（像素）
        
        # 4. 定时器：每0.1秒调用一次 publish_camera_info（10Hz发布频率）
        self.timer = self.create_timer(0.1, self.publish_camera_info)
        
        # 5. 日志提示（确认节点启动）
        self.get_logger().info(
            f"CameraInfo Publisher 1 started! Publishing to: camera_1/camera_info_1"
        )

    def publish_camera_info(self):
        # 创建 CameraInfo 消息对象
        camera_info_msg = CameraInfo()
        
        # 填充消息头部（时间戳 + 坐标系ID）
        camera_info_msg.header.stamp = self.get_clock().now().to_msg()  # 实时时间戳
        camera_info_msg.header.frame_id = 'camera_1_optical_frame'     # 规范的光学坐标系名（按ROS惯例）
        
        # 填充图像尺寸
        camera_info_msg.height = self.image_height
        camera_info_msg.width = self.image_width
        
        # 填充畸变信息
        camera_info_msg.distortion_model = 'plumb_bob'  # 标准畸变模型
        camera_info_msg.d = self.dist_coeffs.tolist()   # numpy数组转Python列表（ROS消息要求）
        
        # 填充内参矩阵 K（3x3矩阵展平为列表）
        camera_info_msg.k = self.camera_matrix.flatten().tolist()
        
        # 填充校正旋转矩阵 R（单目相机默认单位矩阵）
        camera_info_msg.r = [1.0, 0.0, 0.0, 
                             0.0, 1.0, 0.0, 
                             0.0, 0.0, 1.0]
        
        # 填充投影矩阵 P（单目相机默认 [K | 0] 形式）
        camera_info_msg.p = [
            self.camera_matrix[0, 0], self.camera_matrix[0, 1], self.camera_matrix[0, 2], 0.0,
            self.camera_matrix[1, 0], self.camera_matrix[1, 1], self.camera_matrix[1, 2], 0.0,
            self.camera_matrix[2, 0], self.camera_matrix[2, 1], self.camera_matrix[2, 2], 0.0
        ]
        
        # 发布消息
        self.camera_info_publisher.publish(camera_info_msg)

def main(args=None):
    # 初始化 ROS 2 上下文
    rclpy.init(args=args)
    # 创建节点实例
    camera_info_publisher = CameraInfoPublisher()
    # 自旋节点（保持节点运行，处理回调）
    rclpy.spin(camera_info_publisher)
    # 关闭节点并清理 ROS 2 上下文
    camera_info_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
