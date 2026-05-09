#ifndef PATH_GENERATOR_HPP
#define PATH_GENERATOR_HPP

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
#include "skidpad_msgs/msg/car_state.hpp"
#include "skidpad_msgs/msg/trajectory.hpp"
#include "skidpad_msgs/msg/trajectory_point.hpp"
#include "visualization_msgs/msg/marker_array.hpp"
#include "Utils/param.h"
#include "Utils/types.h"
#include "Track/track_base.hpp"
#include "Track/skidpad_track.hpp"

namespace ns_skidpad_planner {

class PathGenerator {
public:
    explicit PathGenerator(rclcpp::Node::SharedPtr node);

    visualization_msgs::msg::MarkerArray getRefPath();
    skidpad_msgs::msg::Trajectory getRefTrajectory();
    visualization_msgs::msg::MarkerArray getGlobalPath();

    void setCarState(const skidpad_msgs::msg::CarState &state);
    void setTransMat(const std_msgs::msg::Float64MultiArray &array);

    void runAlgorithm();

private:
    rclcpp::Node::SharedPtr node_;

    Eigen::Matrix4f transMat_;
    skidpad_msgs::msg::CarState car_state_;
    Trajectory refline_;

    Track *track_;
    Skidpad_Track skidpad_track_;

    visualization_msgs::msg::MarkerArray RefPath_;
    bool transMat_received_ = false;
    bool is_init = false;

    bool Check();
    void setTrack();
};

} // namespace ns_skidpad_planner

#endif