#include "path_generator.hpp"
#include "Utils/visual.h"

namespace ns_skidpad_planner {

PathGenerator::PathGenerator(rclcpp::Node::SharedPtr node) : node_(node) {
    transMat_ = Eigen::Matrix4f::Identity();
    param_.getParams(node_);
    track_ = &skidpad_track_;
}

visualization_msgs::msg::MarkerArray PathGenerator::getRefPath() {
    return RefPath_;
}

skidpad_msgs::msg::Trajectory PathGenerator::getRefTrajectory() {
    skidpad_msgs::msg::Trajectory refTraj_;
    refTraj_.trajectory.clear();
    for (auto &point : refline_) {
        skidpad_msgs::msg::TrajectoryPoint ref_pt;
        ref_pt.pts.x = point.pts.x;
        ref_pt.pts.y = point.pts.y;
        ref_pt.acc.data = point.acc;
        ref_pt.curvature.data = point.curvature;
        ref_pt.r.data = point.r;
        ref_pt.yaw.data = point.yaw;
        ref_pt.velocity.data = point.velocity;
        refTraj_.trajectory.push_back(ref_pt);
    }
    refTraj_.header = car_state_.header;
    refTraj_.remaining_distance.data = track_->getRemainingDistance();
    return refTraj_;
}

visualization_msgs::msg::MarkerArray PathGenerator::getGlobalPath() {
    visualization_msgs::msg::MarkerArray global_path;
    const auto& global_traj = track_->getGlobalTrajectory();
    if (global_traj.empty()) return global_path;

    visualization_msgs::msg::Marker marker;
    marker.header.frame_id = "map";
    marker.header.stamp = car_state_.header.stamp;
    marker.ns = "global_path";
    marker.action = visualization_msgs::msg::Marker::ADD;
    marker.type = visualization_msgs::msg::Marker::LINE_STRIP;
    marker.scale.x = 0.15;
    marker.color.r = 0.5f;
    marker.color.g = 0.5f;
    marker.color.b = 0.5f;
    marker.color.a = 0.3f;
    marker.pose.orientation.w = 1.0;

    for (const auto& pt : global_traj) {
        geometry_msgs::msg::Point p;
        p.x = pt.pts.x;
        p.y = pt.pts.y;
        p.z = 0.0;
        marker.points.push_back(p);
    }
    global_path.markers.push_back(marker);
    return global_path;
}

void PathGenerator::setCarState(const skidpad_msgs::msg::CarState &state) {
    car_state_ = state;
}

void PathGenerator::setTransMat(const std_msgs::msg::Float64MultiArray &array) {
    int element_counter = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            transMat_(i, j) = array.data[element_counter];
            element_counter++;
        }
    }
    transMat_received_ = true;
}

void PathGenerator::runAlgorithm() {
    if (!Check()) {
        return;
    }

    setTrack();

    track_->setState(VehicleState(car_state_));
    track_->CalculateTraj(refline_);

    std::vector<float> color = {1.0f, 0.0f, 0.0f};
    visual_trajectory(refline_, RefPath_, "/base_link", color,
                      car_state_.header, true);
}

bool PathGenerator::Check() {
    return transMat_received_;
}

void PathGenerator::setTrack() {
    if (!is_init) {
        track_->setTransMat(transMat_);
        track_->genTraj();
        is_init = true;
    }
}

} // namespace ns_skidpad_planner