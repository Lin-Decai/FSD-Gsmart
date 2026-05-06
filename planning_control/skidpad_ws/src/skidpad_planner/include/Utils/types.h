#ifndef TYPE_H
#define TYPE_H

#include <iostream>
#include <vector>
#include <cmath>
#include "opencv2/opencv.hpp"
#include "skidpad_msgs/msg/car_state.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"

namespace ns_skidpad_planner {

    struct VehicleState {
        double x;
        double y;
        double yaw;
        double v;
        double r;
        double a;
        double w;

        VehicleState() = default;

        VehicleState(const skidpad_msgs::msg::CarState &state) {
            x = state.car_state.x;
            y = state.car_state.y;
            yaw = state.car_state.theta;
            v = std::hypot(state.car_state_dt.car_state_dt.x,
                           state.car_state_dt.car_state_dt.y);
            r = state.car_state_dt.car_state_dt.theta;
            a = std::hypot(state.car_state_dt.car_state_a.x,
                           state.car_state_dt.car_state_a.y);
            w = state.car_state_dt.car_state_a.theta;
        }
    };

    struct TrajectoryPoint {
        cv::Point2f pts;       // 车辆坐标系坐标 (x向前, y向左)
        double yaw;
        double curvature;
        double velocity;
        double r;
        double acc;
    };

    typedef std::vector<TrajectoryPoint> Trajectory;

} // namespace ns_skidpad_planner

#endif