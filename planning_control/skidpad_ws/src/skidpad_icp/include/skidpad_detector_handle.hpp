#ifndef SKIDPAD_DETECTOR_HANDLE_HPP
#define SKIDPAD_DETECTOR_HANDLE_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>

#include "skidpad_detector.hpp"

namespace ns_skidpad_detector {

class SkidpadDetectorHandle : public rclcpp::Node {
public:
  SkidpadDetectorHandle();

  void init();  // 用于安全创建 skidpad_detector_

  // 手动运行一次（定时器回调中调用 runAlgorithm）
  void run();

private:
  std::string cluster_filtered_topic_name_;
  std::string transform_matrix_topic_name_;
  int node_rate_;

  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr subscriber_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;

  std::unique_ptr<SkidpadDetector> skidpad_detector_;

  void clusterFilteredCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg);
  void timerCallback();
  
  void loadParameters();
  void subscribeToTopics();
  void publishToTopics();
  void sendMsg();
};

} // namespace ns_skidpad_detector

#endif