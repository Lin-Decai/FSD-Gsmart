// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from skidpad_msgs:msg/TrajectoryPoint.idl
// generated code does not contain a copyright notice

#ifndef SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY_POINT__BUILDER_HPP_
#define SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY_POINT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "skidpad_msgs/msg/detail/trajectory_point__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace skidpad_msgs
{

namespace msg
{

namespace builder
{

class Init_TrajectoryPoint_acc
{
public:
  explicit Init_TrajectoryPoint_acc(::skidpad_msgs::msg::TrajectoryPoint & msg)
  : msg_(msg)
  {}
  ::skidpad_msgs::msg::TrajectoryPoint acc(::skidpad_msgs::msg::TrajectoryPoint::_acc_type arg)
  {
    msg_.acc = std::move(arg);
    return std::move(msg_);
  }

private:
  ::skidpad_msgs::msg::TrajectoryPoint msg_;
};

class Init_TrajectoryPoint_r
{
public:
  explicit Init_TrajectoryPoint_r(::skidpad_msgs::msg::TrajectoryPoint & msg)
  : msg_(msg)
  {}
  Init_TrajectoryPoint_acc r(::skidpad_msgs::msg::TrajectoryPoint::_r_type arg)
  {
    msg_.r = std::move(arg);
    return Init_TrajectoryPoint_acc(msg_);
  }

private:
  ::skidpad_msgs::msg::TrajectoryPoint msg_;
};

class Init_TrajectoryPoint_velocity
{
public:
  explicit Init_TrajectoryPoint_velocity(::skidpad_msgs::msg::TrajectoryPoint & msg)
  : msg_(msg)
  {}
  Init_TrajectoryPoint_r velocity(::skidpad_msgs::msg::TrajectoryPoint::_velocity_type arg)
  {
    msg_.velocity = std::move(arg);
    return Init_TrajectoryPoint_r(msg_);
  }

private:
  ::skidpad_msgs::msg::TrajectoryPoint msg_;
};

class Init_TrajectoryPoint_curvature
{
public:
  explicit Init_TrajectoryPoint_curvature(::skidpad_msgs::msg::TrajectoryPoint & msg)
  : msg_(msg)
  {}
  Init_TrajectoryPoint_velocity curvature(::skidpad_msgs::msg::TrajectoryPoint::_curvature_type arg)
  {
    msg_.curvature = std::move(arg);
    return Init_TrajectoryPoint_velocity(msg_);
  }

private:
  ::skidpad_msgs::msg::TrajectoryPoint msg_;
};

class Init_TrajectoryPoint_yaw
{
public:
  explicit Init_TrajectoryPoint_yaw(::skidpad_msgs::msg::TrajectoryPoint & msg)
  : msg_(msg)
  {}
  Init_TrajectoryPoint_curvature yaw(::skidpad_msgs::msg::TrajectoryPoint::_yaw_type arg)
  {
    msg_.yaw = std::move(arg);
    return Init_TrajectoryPoint_curvature(msg_);
  }

private:
  ::skidpad_msgs::msg::TrajectoryPoint msg_;
};

class Init_TrajectoryPoint_pts
{
public:
  Init_TrajectoryPoint_pts()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TrajectoryPoint_yaw pts(::skidpad_msgs::msg::TrajectoryPoint::_pts_type arg)
  {
    msg_.pts = std::move(arg);
    return Init_TrajectoryPoint_yaw(msg_);
  }

private:
  ::skidpad_msgs::msg::TrajectoryPoint msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::skidpad_msgs::msg::TrajectoryPoint>()
{
  return skidpad_msgs::msg::builder::Init_TrajectoryPoint_pts();
}

}  // namespace skidpad_msgs

#endif  // SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY_POINT__BUILDER_HPP_
