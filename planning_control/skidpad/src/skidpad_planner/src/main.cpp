#include "rclcpp/rclcpp.hpp"
#include "path_generator_handle.hpp"

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ns_skidpad_planner::PathGeneratorHandle>();

    rclcpp::Rate loop_rate(node->getNodeRate());
    while (rclcpp::ok()) {
        node->run();
        rclcpp::spin_some(node);
        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}