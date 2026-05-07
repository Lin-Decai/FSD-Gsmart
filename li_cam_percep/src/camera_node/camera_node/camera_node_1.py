#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image, CameraInfo
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
        self.corrected_image_publisher = self.create_publisher(Image, 'camera_1/image_corrected_1', 10)
        
        # ========== 修改部分：内参从话题订阅，不再写死 ==========
        # 内参相关变量（初始为None，等待回调赋值）
        self.camera_matrix = None      # 相机内参矩阵
        self.dist_coeffs = None        # 畸变系数
        self.mapx = None               # 校正映射X
        self.mapy = None               # 校正映射Y
        self.new_camera_matrix = None  # 新相机矩阵
        
        # 是否已初始化校正映射
        self.undistort_ready = False
        
        # 图像尺寸（从内参消息获取）
        self.image_width = None
        self.image_height = None
        
        # 打开默认摄像头
        self.cap = cv2.VideoCapture(1, cv2.CAP_V4L2)
        if not self.cap.isOpened():
            self.get_logger().error("无法打开摄像头")
            return
        
        # ========== 新增：订阅相机内参话题 ==========
        self.camera_info_subscription = self.create_subscription(
            CameraInfo,
            'camera_1/camera_info_1',  # 订阅内参发布节点的话题
            self.camera_info_callback,
            10
        )
        
        # 定时器，以固定频率发布图像
        self.timer = self.create_timer(1/30, self.publish_image)
        
        self.get_logger().info("摄像头节点已启动，等待接收相机内参...")

    def camera_info_callback(self, msg):
        """
        接收相机内参的回调函数
        
        从 CameraInfo 消息中提取内参矩阵和畸变系数，初始化校正映射
        """
        # 检查是否已经初始化过
        if self.undistort_ready:
            return
        
        try:
            # 从消息中提取内参矩阵 K (3x3)
            # msg.k 是展平的列表 [fx, 0, cx, 0, fy, cy, 0, 0, 1]
            k = msg.k
            self.camera_matrix = np.array([
                [k[0], k[1], k[2]],
                [k[3], k[4], k[5]],
                [k[6], k[7], k[8]]
            ], dtype=np.float64)
            
            # 从消息中提取畸变系数 D
            # 标准 plumb_bob 模型有5个系数 [k1, k2, p1, p2, k3]
            self.dist_coeffs = np.array(msg.d, dtype=np.float64)
            
            # 使用内参消息中的图像尺寸
            self.image_width = msg.width
            self.image_height = msg.height
            
            # 获取摄像头实际尺寸（用于校正映射）
            cap_width = int(self.cap.get(cv2.CAP_PROP_FRAME_WIDTH))
            cap_height = int(self.cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
            
            # 如果摄像头尺寸与内参尺寸不一致，使用摄像头尺寸并给出警告
            if cap_width != self.image_width or cap_height != self.image_height:
                self.get_logger().warn(
                    f"摄像头尺寸({cap_width}x{cap_height})与内参尺寸({self.image_width}x{self.image_height})不一致，"
                    f"使用摄像头尺寸进行校正"
                )
                width, height = cap_width, cap_height
            else:
                width, height = self.image_width, self.image_height
            
            # 计算新的相机矩阵（保留全部视野）
            self.new_camera_matrix, _ = cv2.getOptimalNewCameraMatrix(
                self.camera_matrix, self.dist_coeffs, (width, height), 1, (width, height)
            )
            
            # 初始化校正映射
            self.mapx, self.mapy = cv2.initUndistortRectifyMap(
                self.camera_matrix, self.dist_coeffs, np.eye(3),
                self.new_camera_matrix, (width, height), 5
            )
            
            self.undistort_ready = True
            
            self.get_logger().info(
                f"相机内参已接收并初始化校正映射\n"
                f"内参矩阵:\n{self.camera_matrix}\n"
                f"畸变系数: {self.dist_coeffs}\n"
                f"图像尺寸: {width}x{height}"
            )
            
        except Exception as e:
            self.get_logger().error(f"初始化校正映射失败: {e}")

    def publish_image(self):
        """发布原始图像和校正后的图像"""
        # 等待内参就绪
        if not self.undistort_ready:
            # 内参还未收到，跳过发布
            return
        
        # 读取摄像头帧
        ret, frame = self.cap.read()
        if not ret:
            self.get_logger().error("无法从摄像头获取帧")
            return
        
        # 旋转图像180度
        rotated_frame = cv2.rotate(frame, cv2.ROTATE_180)
        
        # 发布原始图像
        self.publish_raw_image(rotated_frame)
        
        # 校正图像畸变并发布
        self.publish_corrected_image(rotated_frame)

    def publish_raw_image(self, frame):
        """发布原始图像"""
        try:
            image_msg = self.bridge.cv2_to_imgmsg(frame, "bgr8")
            self.image_publisher.publish(image_msg)
        except Exception as e:
            self.get_logger().error(f"转换原始图像失败: {e}")

    def publish_corrected_image(self, frame):
        """校正并发布图像"""
        try:
            # 使用预计算的映射进行畸变校正
            undistorted_frame = cv2.remap(
                frame, self.mapx, self.mapy, cv2.INTER_LINEAR
            )
            
            # 转换为ROS消息并发布
            corrected_image_msg = self.bridge.cv2_to_imgmsg(undistorted_frame, "bgr8")
            self.corrected_image_publisher.publish(corrected_image_msg)
        except Exception as e:
            self.get_logger().error(f"转换校正图像失败: {e}")

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
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()