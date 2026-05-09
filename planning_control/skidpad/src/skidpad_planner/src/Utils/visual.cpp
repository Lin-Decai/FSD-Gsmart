#include "Utils/visual.h"
#include "Utils/param.h" 
#include "rclcpp/rclcpp.hpp"

extern Param param_;   // 声明外部全局参数变量

namespace ns_skidpad_planner {

void visual_trajectory(const Trajectory &traj,
                       visualization_msgs::msg::MarkerArray &visual,
                       const std::string &frame,
                       const std::vector<float> &color,
                       const std_msgs::msg::Header &header,
                       bool is_vel)
{
    visual.markers.clear();
    std::vector<float> color_tmp;

    for (size_t i = 0; i < traj.size() - 1; i++) {
        visualization_msgs::msg::Marker tmp;
        tmp.header.frame_id = frame;
        tmp.header.stamp = header.stamp;
        tmp.ns = "line_" + std::to_string(i);
        tmp.action = visualization_msgs::msg::Marker::ADD;
        tmp.type = visualization_msgs::msg::Marker::LINE_STRIP;
        tmp.scale.x = 0.4;

        if (is_vel)
            color_map(traj[i].velocity, color_tmp);
        else
            color_tmp = color;

        tmp.color.r = color_tmp[0];
        tmp.color.g = color_tmp[1];
        tmp.color.b = color_tmp[2];
        tmp.color.a = 1.0;

        geometry_msgs::msg::Point p;
        p.x = traj[i].pts.x;
        p.y = traj[i].pts.y;
        tmp.points.push_back(p);
        p.x = traj[i + 1].pts.x;
        p.y = traj[i + 1].pts.y;
        tmp.points.push_back(p);

        visual.markers.push_back(tmp);
    }
}

void color_map(double vel, std::vector<float> &color) {
    double temp = (vel - param_.initial_velocity) /
                  (param_.desire_vel - param_.initial_velocity);
    if (temp > 1) temp = 1;
    if (temp < 0) temp = 0;
    color.resize(3);
    color[0] = temp;
    color[1] = 0;
    color[2] = 1 - temp;
}

} // namespace ns_skidpad_planner