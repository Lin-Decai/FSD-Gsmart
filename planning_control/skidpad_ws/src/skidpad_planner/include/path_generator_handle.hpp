#ifndef PATH_GENERATOR_HANDLE_HPP
#define PATH_GENERATOR_HANDLE_HPP

#include "rclcpp/rclcpp.hpp"
#include "path_generator.hpp"
#include "skidpad_msgs/msg/car_state.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"

namespace ns_skidpad_planner {

class PathGeneratorHandle : public rclcpp::Node {
public:
    PathGeneratorHandle();

    void run();

    // 获取循环频率
    int getNodeRate() const { return node_rate_; }

private:
    void loadParameters();
    void setupSubscribers();
    void setupPublishers();
    void sendMsg();

    void carStateCallback(const skidpad_msgs::msg::CarState::SharedPtr msg);
    void transMatCallback(const std_msgs::msg::Float64MultiArray::SharedPtr msg);

    std::string car_state_topic_name_;
    std::string transform_matrix_topic_name_;
    std::string ref_path_topic_name_;
    std::string path_generate_topic_name_;
    std::string global_path_topic_name_;
    int node_rate_;

    rclcpp::Subscription<skidpad_msgs::msg::CarState>::SharedPtr carStateSubscriber_;
    rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr transMatSubscriber_;

    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr refPathVisualPublisher_;
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr globalPathVisualPublisher_;
    rclcpp::Publisher<skidpad_msgs::msg::Trajectory>::SharedPtr refPathPublisher_;

    PathGenerator path_generator_;
};

} // namespace ns_skidpad_planner

#endif