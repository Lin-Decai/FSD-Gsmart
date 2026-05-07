#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2
import numpy as np
from camera_msg.msg import YoloMsg, YoloMsgArray  # 修改导入路径
from ultralytics import YOLO
import traceback

class YoloCameraNode(Node):
    def __init__(self):
        super().__init__('yolo_camera_node')
        
        # 初始化OpenCV桥接器
        self.bridge = CvBridge()
        
        # 创建订阅者和发布者
        self.image_sub = self.create_subscription(
            Image,
            'camera_2/image_corrected_2',
            self.image_callback,
            30)
        
        self.yolo_pub = self.create_publisher(
            YoloMsgArray,
            '/yolo/detections_2',
            30)
        
        # 加载YOLO模型（从参数获取路径）
        model_path = self.declare_parameter('model_path', 
                                           '/media/nvidia/nvme0n1/gsmart_2025/best0918.engine').value
        confidence = self.declare_parameter('confidence', 0.5).value
        
        self.get_logger().info(f"加载YOLO模型: {model_path}, 置信度阈值: {confidence}")
        try:
            self.model = YOLO(model_path)
            self.get_logger().info("YOLO模型加载完成")
        except Exception as e:
            self.get_logger().fatal(f"模型加载失败: {str(e)}\n{traceback.format_exc()}")
            rclpy.shutdown()  # 优雅地关闭ROS 2
            return
        
        # 启动定时器
        self.timer = self.create_timer(1.0, self.timer_callback)
        self.get_logger().info("YOLO相机节点已启动，等待图像输入...")

    def timer_callback(self):
        """定时器回调：显示节点运行状态"""
        self.get_logger().debug("节点运行中...")

    def image_callback(self, msg):
        """图像回调：处理图像并进行目标检测"""
        try:
            # 转换图像格式
            cv_image = self.bridge.imgmsg_to_cv2(msg, "bgr8")
            
            # 进行目标检测
            confidence = self.get_parameter('confidence').value
            results = self.model.predict(
                cv_image, show=False, stream=True, 
                conf=confidence, line_width=1
            )
            
            # 构建检测结果消息
            msg_array = YoloMsgArray()
            msg_array.header = msg.header  # 仅在数组级别保留header
            
            for r in results:
                boxes = r.boxes.xyxy.tolist()
                classes = r.boxes.cls.tolist()
                confidences = r.boxes.conf.tolist()
                
                for box, cls, conf in zip(boxes, classes, confidences):
                    yolo_msg = YoloMsg()
                    yolo_msg.xmin = int(box[0])
                    yolo_msg.ymin = int(box[1])
                    yolo_msg.xmax = int(box[2])
                    yolo_msg.ymax = int(box[3])
                    yolo_msg.label = float(cls)
                    yolo_msg.confidence = conf
                    
                    # 修改这一行，使用正确的字段名 'cones'
                    msg_array.cones.append(yolo_msg)
            
            # 发布检测结果
            self.yolo_pub.publish(msg_array)
            
        except Exception as e:
            error_msg = traceback.format_exc()
            self.get_logger().error(f"图像处理错误: {str(e)}\n{error_msg}")

    def destroy_node(self):
        """节点销毁时释放资源"""
        cv2.destroyAllWindows()  # 释放OpenCV窗口
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = YoloCameraNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info("节点被用户中断")
    except Exception as e:
        error_msg = traceback.format_exc()
        node.get_logger().error(f"主程序错误: {str(e)}\n{error_msg}")
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()

