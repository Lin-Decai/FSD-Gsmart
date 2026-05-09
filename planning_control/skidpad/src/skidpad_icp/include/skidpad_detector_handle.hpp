#ifndef SKIDPAD_DETECTOR_HANDLE_HPP
#define SKIDPAD_DETECTOR_HANDLE_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_array.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>

#include "skidpad_detector.hpp"

namespace ns_skidpad_detector {

class SkidpadDetectorHandle : public rclcpp::Node {
public:
  SkidpadDetectorHandle();

  void init();

  void run();

private:
  std::string cluster_filtered_topic_name_;
  std::string transform_matrix_topic_name_;
  int node_rate_;

  rclcpp::Subscription<geometry_msgs::msg::PoseArray>::SharedPtr subscriber_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;

  std::unique_ptr<SkidpadDetector> skidpad_detector_;

  void clusterFilteredCallback(const geometry_msgs::msg::PoseArray::SharedPtr msg);
  void timerCallback();

  void loadParameters();
  void subscribeToTopics();
  void publishToTopics();
  void sendMsg();
};

} // namespace ns_skidpad_detector

#endif
