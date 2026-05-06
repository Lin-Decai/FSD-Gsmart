#pragma once

#include "Utils/types.h"
#include "Utils/param.h"
#include "geometry_msgs/msg/point.hpp"
#include "Eigen/Dense"
#include <vector>

namespace ns_skidpad_planner {

class Track {
public:
    Track() = default;
    virtual ~Track() = default;

    virtual bool genTraj() = 0;
    virtual bool CalculateTraj(Trajectory &refline) = 0;

    void setState(const VehicleState &state);
    void setTransMat(const Eigen::Matrix4f &transMat);

    double getRemainingDistance() const { return remaining_distance_; }
    const Trajectory& getGlobalTrajectory() const { return trajectory_; }

protected:
    VehicleState state_;
    Trajectory trajectory_;
    Eigen::Matrix4f transMat_;
    double remaining_distance_ = 0.0;
};

} // namespace ns_skidpad_planner