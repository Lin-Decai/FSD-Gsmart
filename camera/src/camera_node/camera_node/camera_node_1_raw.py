#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2
import numpy as np

class CameraNode(Node):
    def __init__(self):
        super().__init__('camera_node_1')
        
        # 初始化CV桥
        self.bridge = CvBridge()
        
        # 创建发布者，发布原始图像和校正后的图像话题
        self.image_publisher = self.create_publisher(Image, 'camera_1/image_raw_1', 10)
        
        # 打开默认摄像头（通常为0）
        self.cap = cv2.VideoCapture(1,cv2.CAP_V4L2)
        if not self.cap.isOpened():
            self.get_logger().error("无法打开摄像头")
            return
            
       
        
        # 定时器，以固定频率发布图像
        self.timer = self.create_timer(1/15, self.publish_image)  # 30Hz
            
        self.get_logger().info("摄像头节点已启动")


    def publish_image(self):
        """发布原始图像和校正后的图像"""
        # 读取摄像头帧
        ret, frame = self.cap.read()
        if not ret:
            self.get_logger().error("无法从摄像头获取帧")
            return
            
        # 旋转图像180度
        rotated_frame = cv2.rotate(frame, cv2.ROTATE_180)
        
        # 发布原始图像
        self.publish_raw_image(rotated_frame)
        
       

    def publish_raw_image(self, frame):
        """发布原始图像"""
        try:
            image_msg = self.bridge.cv2_to_imgmsg(frame, "bgr8")
            self.image_publisher.publish(image_msg)
            self.get_logger().debug("发布了一帧原始图像")
        except Exception as e:
            self.get_logger().error(f"转换原始图像失败: {e}")

    

    def __del__(self):
        # 释放摄像头资源
        if hasattr(self, 'cap') and self.cap.isOpened():
            self.cap.release()

def main(args=None):
    rclpy.init(args=args)
    node = CameraNode()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        # 清理资源
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()



