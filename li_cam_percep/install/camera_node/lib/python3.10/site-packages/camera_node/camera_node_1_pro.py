#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image, CameraInfo
from cv_bridge import CvBridge
import cv2
import numpy as np

class CameraNode(Node):
    def __init__(self):
        super().__init__('camera_node_1_pro_pro')
        
        # 初始化CV桥
        self.bridge = CvBridge()
        
        # 创建发布者
        self.corrected_image_publisher = self.create_publisher(Image, 'camera_1/image_corrected_1', 30)
        
        # ========== 修改部分：内参从话题订阅，不再写死 ==========
        # 内参相关变量（初始为None，等待回调赋值）
        self.camera_matrix = None      # 相机内参矩阵
        self.dist_coeffs = None        # 畸变系数
        self.mapx = None               # 校正映射X
        self.mapy = None               # 校正映射Y
        self.new_camera_matrix = None  # 新相机矩阵
        
        # 是否已初始化校正映射
        self.undistort_ready = False
        
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
        self.timer = self.create_timer(1/30, self.publish_corrected_image)
        
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
            
            # 获取图像尺寸
            width = int(self.cap.get(cv2.CAP_PROP_FRAME_WIDTH))
            height = int(self.cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
            
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
                f"畸变系数: {self.dist_coeffs}"
            )
            
        except Exception as e:
            self.get_logger().error(f"初始化校正映射失败: {e}")

    def publish_corrected_image(self):
        """读取、校正并发布图像"""
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
        
        try:
            # 使用预计算的映射进行畸变校正
            undistorted_frame = cv2.remap(
                rotated_frame, self.mapx, self.mapy, cv2.INTER_LINEAR
            )
            
            # 转换为ROS消息并发布
            corrected_image_msg = self.bridge.cv2_to_imgmsg(undistorted_frame, "bgr8")
            self.corrected_image_publisher.publish(corrected_image_msg)
            
        except Exception as e:
            self.get_logger().error(f"处理图像失败: {e}")

    def __del__(self):
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