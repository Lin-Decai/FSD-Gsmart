#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import CameraInfo
import numpy as np

class CameraInfoPublisher(Node):

    def __init__(self):
        # --- 1. 【已修复】调用父类 Node 的构造函数 ---
        # 必须先调用 super().__init__() 来完成 Node 的初始化
        super().__init__('camera_info_publisher_2')
        
        # --- 2. 创建发布者 ---
        self.camera_info_publisher = self.create_publisher(
            CameraInfo, 
            'camera_2/camera_info_2', 
            10  
        )
        
        # --- 3. 相机参数 ---
        self.camera_matrix = np.array([[1926.06478016793, 0, 881.19],
                                       [0, 1927.75557277439, 497.4134],
                                       [0, 0, 1]])

        self.dist_coeffs = np.array([-0.526848084119857, 0.279619545278389, 0.00362168285490537, 0.000267399425785593, -0.0872414844157106])
        self.image_width = 1920
        self.image_height = 1080
        
        # --- 4. 设置定时器 ---
        self.timer = self.create_timer(0.1, self.publish_camera_info)
        
        # --- 5. 【已优化】修正日志消息 ---
        self.get_logger().info('CameraInfo Publisher Node 2 has been started. Publishing to: %s' % 'camera_2/camera_info_2')

    def publish_camera_info(self):
        camera_info_msg = CameraInfo()
        camera_info_msg.header.stamp = self.get_clock().now().to_msg()
        
        # --- 6. 【已优化】使用规范的 frame_id ---
        camera_info_msg.header.frame_id = 'camera_2_optical_frame'
        
        camera_info_msg.height = self.image_height
        camera_info_msg.width = self.image_width
        camera_info_msg.distortion_model = 'plumb_bob'
        camera_info_msg.d = self.dist_coeffs.tolist()
        camera_info_msg.k = self.camera_matrix.flatten().tolist()
        
        camera_info_msg.r = [1.0, 0.0, 0.0, 
                             0.0, 1.0, 0.0, 
                             0.0, 0.0, 1.0]
        
        camera_info_msg.p = [
            self.camera_matrix[0, 0], self.camera_matrix[0, 1], self.camera_matrix[0, 2], 0.0,
            self.camera_matrix[1, 0], self.camera_matrix[1, 1], self.camera_matrix[1, 2], 0.0,
            self.camera_matrix[2, 0], self.camera_matrix[2, 1], self.camera_matrix[2, 2], 0.0
        ]
        
        self.camera_info_publisher.publish(camera_info_msg)


def main(args=None):
    rclpy.init(args=args)
    camera_info_publisher = CameraInfoPublisher()
    rclpy.spin(camera_info_publisher)
    camera_info_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
