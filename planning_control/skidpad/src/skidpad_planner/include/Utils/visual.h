#pragma once
#include "visualization_msgs/msg/marker_array.hpp"
#include "Utils/types.h"
#include "std_msgs/msg/header.hpp"
#include <vector>
#include <string>

namespace ns_skidpad_planner {

void visual_trajectory(const Trajectory &traj,
                       visualization_msgs::msg::MarkerArray &visual,
                       const std::string &frame,
                       const std::vector<float> &color,
                       const std_msgs::msg::Header &header,
                       bool is_vel);

void color_map(double vel, std::vector<float> &color);

} // namespace ns_skidpad_planner