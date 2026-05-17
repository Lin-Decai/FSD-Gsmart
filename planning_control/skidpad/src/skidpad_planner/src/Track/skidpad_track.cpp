// 8字绕环全局轨迹生成 + 局部规划 (坐标: X=直道=北, Y=左=西)
#include "Track/skidpad_track.hpp"
#include "rclcpp/rclcpp.hpp"

Param param_;

namespace ns_skidpad_planner {

// ============================================================================
//  全局轨迹生成: 直道→右圈2圈→左圈2圈→直道
//  切点/圆心: X=forward_distance=15.00
//        右圈 (15.00, -9.125), 左圈 (15.00, +9.125)
// ============================================================================
bool Skidpad_Track::genTraj() {
    double interval             = param_.interval;
    double forward_distance     = param_.forward_distance;
    double circle_radius        = param_.circle_radius;
    const double circle_curv    = 1.0 / circle_radius;

    double right_circle_x = forward_distance;
    double right_circle_y = -circle_radius;
    double left_circle_x  = right_circle_x;
    double left_circle_y  = circle_radius;

    TrajectoryPoint tmp_pt;
    trajectory_.clear();

    // 直线段1: 从车后2m到切点 (forward_distance=15.00m)
    for (double i = -2.0; i < forward_distance; i += interval) {
        tmp_pt.pts = cv::Point2f(i, 0);
        tmp_pt.yaw = 0;
        tmp_pt.curvature = 0.0;
        tmp_pt.velocity = 0.0;
        tmp_pt.r = 0.0;
        tmp_pt.acc = 0.0;
        trajectory_.push_back(tmp_pt);
    }
    idx_end_straight1_ = trajectory_.size();

    // 右圈: 顺时针2圈 (i=0到4π, 从切点入, 先向南)
    for (double i = 0; i < 4 * M_PI; i += interval / circle_radius) {
        tmp_pt.pts.x = circle_radius * std::cos(90 * M_PI / 180 - i) + right_circle_x;
        tmp_pt.pts.y = circle_radius * std::sin(90 * M_PI / 180 - i) + right_circle_y;
        tmp_pt.yaw = -i;  // 顺时针, yaw递减
        tmp_pt.curvature = circle_curv;
        tmp_pt.velocity = 0.0;
        tmp_pt.r = 0.0;
        tmp_pt.acc = 0.0;
        trajectory_.push_back(tmp_pt);
    }
    idx_end_right_ = trajectory_.size();

    // 左圈: 逆时针2圈 (i=0到4π, 从切点入, 先向北)
    for (double i = 0; i < 4 * M_PI; i += interval / circle_radius) {
        tmp_pt.pts.x = circle_radius * std::cos(-90 * M_PI / 180 + i) + left_circle_x;
        tmp_pt.pts.y = circle_radius * std::sin(-90 * M_PI / 180 + i) + left_circle_y;
        tmp_pt.yaw = i;  // 逆时针, yaw递增
        tmp_pt.curvature = circle_curv;
        tmp_pt.velocity = 0.0;
        tmp_pt.r = 0.0;
        tmp_pt.acc = 0.0;
        trajectory_.push_back(tmp_pt);
    }
    idx_end_left_ = trajectory_.size();

    // 直线段2: 从切点驶出15m (forward_distance → forward_distance+15)
    for (double i = forward_distance;
         i < forward_distance + 15.0;
         i += interval) {
        tmp_pt.pts = cv::Point2f(i, 0);
        tmp_pt.yaw = 0;
        tmp_pt.curvature = 0.0;
        tmp_pt.velocity = 0.0;
        tmp_pt.r = 0.0;
        tmp_pt.acc = 0.0;
        trajectory_.push_back(tmp_pt);
    }

    // ICP 变换: 将理论轨迹对齐到真实世界
    for (size_t i = 0; i < trajectory_.size(); i++) {
        Eigen::Vector4f temp(trajectory_[i].pts.x,
                             trajectory_[i].pts.y, 0, 1);
        Eigen::Vector4f result = transMat_ * temp;
        trajectory_[i].pts.x = result[0] / result[3];
        trajectory_[i].pts.y = result[1] / result[3];
    }

    return true;
}

// ---------------------------------------------------------------------------
//  赛道阶段判定 (基于轨迹点索引)
//  0=直行起跑 1=右圈第1圈 2=右圈第2圈 3=左圈第1圈 4=左圈第2圈 5=直行驶出
// ---------------------------------------------------------------------------
int Skidpad_Track::computePhase(size_t index) const {
    if (index < idx_end_straight1_) return 0;
    size_t right_len = idx_end_right_ - idx_end_straight1_;
    size_t right_half = right_len / 2;
    if (index < idx_end_straight1_ + right_half) return 1;
    if (index < idx_end_right_) return 2;
    size_t left_len = idx_end_left_ - idx_end_right_;
    size_t left_half = left_len / 2;
    if (index < idx_end_right_ + left_half) return 3;
    if (index < idx_end_left_) return 4;
    return 5;
}

// ============================================================================
//  局部规划: 从车辆当前位置沿全局轨迹向前采样N个点, 转换到车身坐标系
//  refline = 输出的局部轨迹 (车身坐标系: X=前, Y=左)
// ============================================================================
bool Skidpad_Track::CalculateTraj(Trajectory &refline) {
    if (trajectory_.empty()) {
        RCLCPP_WARN(rclcpp::get_logger("skidpad"), "Trajectory is empty !");
        return false;
    }

    refline.clear();

    double px  = state_.x;    // 车辆全局X
    double py  = state_.y;    // 车辆全局Y
    double psi = state_.yaw;  // 车辆航向

    // 第1步: 在全局轨迹上搜索距离车辆最近的点 (搜索窗口: now_state ~ now_state+400)
    int index_min = -1;
    double min = 9999.0;
    for (int i = now_state; i < static_cast<int>(trajectory_.size()) && i < now_state + 400; i++) {
        double delta_x = trajectory_[i].pts.x - px;
        double delta_y = trajectory_[i].pts.y + py;
        double dist = std::hypot(delta_x, delta_y);
        if (dist < min) {
            min = dist;
            index_min = i;
        }
    }

    if (index_min < 0) return false;

    now_state = index_min;

    // 赛道阶段检测 (用于终端打印 [STATE])
    current_phase_ = computePhase(now_state);
    if (current_phase_ != prev_phase_) {
        prev_phase_ = current_phase_;
        const char* labels[] = {"Straight-line start", "Right circle lap 1", "Right circle lap 2",
                                "Left circle lap 1", "Left circle lap 2", "Straight-line exit"};
        RCLCPP_INFO(rclcpp::get_logger("skidpad_planner"),
                    "[STATE] %s", labels[current_phase_]);
    }

    // 计算剩余弧长 (从当前点到轨迹终点)
    remaining_distance_ = 0.0;
    for (size_t i = index_min; i + 1 < trajectory_.size(); i++) {
        double dx = trajectory_[i + 1].pts.x - trajectory_[i].pts.x;
        double dy = trajectory_[i + 1].pts.y - trajectory_[i].pts.y;
        remaining_distance_ += std::hypot(dx, dy);
    }

    // 第2步: 从最近点向前采样N个点, 间距 = v*dt, 转换到车身坐标系
    const double desired_velocity = param_.desire_vel;
    const int    N                = param_.N;
    const double dt               = param_.dt;

    double v = std::fmax(state_.v, param_.initial_velocity);  // 确保起步时有预瞄
    double s_tmp = 0.0;  // 沿轨迹的累计弧长

    for (int i = 0; i < N; i++) {
        if (i != 0) s_tmp += v * dt;

        int index = static_cast<int>(s_tmp / param_.interval);
        int next = index + index_min;
        if (next >= static_cast<int>(trajectory_.size())) next = trajectory_.size() - 1;

        // 全局 → 车身坐标系变换
        double delta_x   = trajectory_[next].pts.x - px;
        double delta_y   = trajectory_[next].pts.y + py;
        double delta_yaw = trajectory_[next].yaw - psi;

        while (delta_yaw >= M_PI) delta_yaw -= 2 * M_PI;
        while (delta_yaw <= -M_PI) delta_yaw += 2 * M_PI;

        // 旋转: (delta_x, delta_y) 绕原点转 -psi → 车身坐标
        double temp_x = delta_x * cos(psi) + delta_y * sin(psi);   // 前方
        double temp_y = delta_y * cos(psi) - delta_x * sin(psi);   // 左侧

        TrajectoryPoint tmp;
        tmp.pts = cv::Point2f(temp_x, temp_y);
        tmp.yaw = delta_yaw;
        tmp.curvature = fabs(trajectory_[next].curvature);

        // 弯道限速: v_max = sqrt(max_lat_acc / curvature)
        double v_max_curve = std::numeric_limits<double>::infinity();
        if (tmp.curvature > 1e-6) {
            v_max_curve = sqrt(param_.max_lat_acc / tmp.curvature);
        }
        tmp.velocity = std::min(v_max_curve, desired_velocity);
        tmp.r = 0.0;
        tmp.acc = 0.0;

        refline.push_back(tmp);

        v = tmp.velocity;  // 更新速度用于下一步步长计算
    }

    return true;
}
} // namespace ns_skidpad_planner
