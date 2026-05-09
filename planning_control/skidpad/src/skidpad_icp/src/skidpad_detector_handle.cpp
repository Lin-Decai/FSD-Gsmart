#include "skidpad_detector_handle.hpp"

#include <pcl_conversions/pcl_conversions.h>

namespace ns_skidpad_detector {

SkidpadDetectorHandle::SkidpadDetectorHandle() : Node("skidpad_detector_node") {
  RCLCPP_INFO(this->get_logger(), "Constructing Handle");
  
  loadParameters();
  subscribeToTopics();
  publishToTopics();
  
  timer_ = this->create_wall_timer(
      std::chrono::milliseconds(1000 / node_rate_),
      std::bind(&SkidpadDetectorHandle::timerCallback, this));
}

void SkidpadDetectorHandle::init() {
    // 此时对象已经被 shared_ptr 完全管理，shared_from_this() 安全
    // 创建核心算法对象，传递当前节点的共享指针（用于访问参数和日志）
    skidpad_detector_ = std::make_unique<SkidpadDetector>(shared_from_this()); 
}

void SkidpadDetectorHandle::loadParameters() {
  RCLCPP_INFO(this->get_logger(), "loading handle parameters");
  this->declare_parameter<std::string>("cluster_filtered_topic_name", "/hesai/cone_positions");
  this->declare_parameter<std::string>("transform_matrix_topic_name", "/transform_matrix");
  this->declare_parameter<int>("node_rate", 50);
  
  this->get_parameter("cluster_filtered_topic_name", cluster_filtered_topic_name_);
  this->get_parameter("transform_matrix_topic_name", transform_matrix_topic_name_);
  this->get_parameter("node_rate", node_rate_);
}

void SkidpadDetectorHandle::subscribeToTopics() {
  RCLCPP_INFO(this->get_logger(), "subscribe to topics");
  subscriber_ = this->create_subscription<geometry_msgs::msg::PoseArray>(
      cluster_filtered_topic_name_, 10,
      std::bind(&SkidpadDetectorHandle::clusterFilteredCallback, this, std::placeholders::_1));
}

void SkidpadDetectorHandle::publishToTopics() {
  RCLCPP_INFO(this->get_logger(), "publish to topics");
  publisher_ = this->create_publisher<std_msgs::msg::Float64MultiArray>(
      transform_matrix_topic_name_, 10);
}

void SkidpadDetectorHandle::timerCallback() {
  skidpad_detector_->runAlgorithm();
  sendMsg();
}

void SkidpadDetectorHandle::sendMsg() {
  if (skidpad_detector_->getMatchFlag()) {
    publisher_->publish(skidpad_detector_->getTransMatrix());
  }
}

void SkidpadDetectorHandle::clusterFilteredCallback(const geometry_msgs::msg::PoseArray::SharedPtr msg) {
  pcl::PointCloud<pcl::PointXYZ> cloud;
  cloud.points.reserve(msg->poses.size());

  for (const auto& pose : msg->poses) {
    pcl::PointXYZ pt;
    pt.x = pose.position.x;
    pt.y = pose.position.y;
    pt.z = pose.position.z;
    cloud.points.push_back(pt);
  }

  auto cloud_msg = std::make_shared<sensor_msgs::msg::PointCloud2>();
  pcl::toROSMsg(cloud, *cloud_msg);
  cloud_msg->header = msg->header;

  skidpad_detector_->setclusterFiltered(cloud_msg);
}

void SkidpadDetectorHandle::run() {
  // 定时器驱动，无需额外操作
}

} // namespace ns_skidpad_detector