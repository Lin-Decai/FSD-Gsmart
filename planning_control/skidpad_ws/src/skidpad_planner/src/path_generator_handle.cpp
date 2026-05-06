#include "path_generator_handle.hpp"

namespace ns_skidpad_planner {

PathGeneratorHandle::PathGeneratorHandle()
    : Node("path_generator"),
      path_generator_(std::shared_ptr<rclcpp::Node>(this, [](auto*){}))
{
    RCLCPP_INFO(get_logger(), "Constructing Handle");
    loadParameters();
    setupSubscribers();
    setupPublishers();
}

void PathGeneratorHandle::loadParameters() {
    car_state_topic_name_ = declare_parameter<std::string>(
        "car_state_topic_name", "/estimation/slam/state");
    transform_matrix_topic_name_ = declare_parameter<std::string>(
        "transform_matrix_topic_name", "/transform_matrix");
    ref_path_topic_name_ = declare_parameter<std::string>(
        "ref_path_topic_name", "/visual/ref_path");
    global_path_topic_name_ = declare_parameter<std::string>(
        "global_path_topic_name", "/visual/global_path");
    path_generate_topic_name_ = declare_parameter<std::string>(
        "path_generate_topic_name", "/planning/ref_path");
    node_rate_ = declare_parameter<int>("node_rate", 100);
}

void PathGeneratorHandle::setupSubscribers() {
    using namespace std::placeholders;
    carStateSubscriber_ = create_subscription<skidpad_msgs::msg::CarState>(
        car_state_topic_name_, 10,
        std::bind(&PathGeneratorHandle::carStateCallback, this, _1));
    transMatSubscriber_ = create_subscription<std_msgs::msg::Float64MultiArray>(
        transform_matrix_topic_name_, 10,
        std::bind(&PathGeneratorHandle::transMatCallback, this, _1));
}

void PathGeneratorHandle::setupPublishers() {
    refPathVisualPublisher_ = create_publisher<visualization_msgs::msg::MarkerArray>(
        ref_path_topic_name_, 10);
    globalPathVisualPublisher_ = create_publisher<visualization_msgs::msg::MarkerArray>(
        global_path_topic_name_, 10);
    refPathPublisher_ = create_publisher<skidpad_msgs::msg::Trajectory>(
        path_generate_topic_name_, 10);
}

void PathGeneratorHandle::carStateCallback(const skidpad_msgs::msg::CarState::SharedPtr msg) {
    path_generator_.setCarState(*msg);
}

void PathGeneratorHandle::transMatCallback(const std_msgs::msg::Float64MultiArray::SharedPtr msg) {
    path_generator_.setTransMat(*msg);
}

void PathGeneratorHandle::run() {
    path_generator_.runAlgorithm();
    sendMsg();
}

void PathGeneratorHandle::sendMsg() {
    refPathVisualPublisher_->publish(path_generator_.getRefPath());
    globalPathVisualPublisher_->publish(path_generator_.getGlobalPath());
    refPathPublisher_->publish(path_generator_.getRefTrajectory());
}

} // namespace ns_skidpad_planner