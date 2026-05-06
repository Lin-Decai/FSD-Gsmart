#include <rclcpp/rclcpp.hpp>  // ROS2 核心
#include "skidpad_detector_handle.hpp"  // 我们的节点类

int main(int argc, char **argv) {
  // 初始化 ROS2
  rclcpp::init(argc, argv);

  // 创建节点对象（即 SkidpadDetectorHandle 实例）
  auto node = std::make_shared<ns_skidpad_detector::SkidpadDetectorHandle>();
  
  // 在构造函数之后调用init()函数 来创建核心算法对象
  node->init();   

  // 进入事件循环，ROS2 会在此处等待并处理订阅回调、定时器等
  rclcpp::spin(node);

  // 退出前关闭 ROS2
  rclcpp::shutdown();

  return 0;
}

