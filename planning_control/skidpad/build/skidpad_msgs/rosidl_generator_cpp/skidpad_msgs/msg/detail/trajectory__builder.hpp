// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from skidpad_msgs:msg/Trajectory.idl
// generated code does not contain a copyright notice

#ifndef SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY__BUILDER_HPP_
#define SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "skidpad_msgs/msg/detail/trajectory__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace skidpad_msgs
{

namespace msg
{

namespace builder
{

class Init_Trajectory_remaining_distance
{
public:
  explicit Init_Trajectory_remaining_distance(::skidpad_msgs::msg::Trajectory & msg)
  : msg_(msg)
  {}
  ::skidpad_msgs::msg::Trajectory remaining_distance(::skidpad_msgs::msg::Trajectory::_remaining_distance_type arg)
  {
    msg_.remaining_distance = std::move(arg);
    return std::move(msg_);
  }

private:
  ::skidpad_msgs::msg::Trajectory msg_;
};

class Init_Trajectory_trajectory
{
public:
  explicit Init_Trajectory_trajectory(::skidpad_msgs::msg::Trajectory & msg)
  : msg_(msg)
  {}
  Init_Trajectory_remaining_distance trajectory(::skidpad_msgs::msg::Trajectory::_trajectory_type arg)
  {
    msg_.trajectory = std::move(arg);
    return Init_Trajectory_remaining_distance(msg_);
  }

private:
  ::skidpad_msgs::msg::Trajectory msg_;
};

class Init_Trajectory_header
{
public:
  Init_Trajectory_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Trajectory_trajectory header(::skidpad_msgs::msg::Trajectory::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Trajectory_trajectory(msg_);
  }

private:
  ::skidpad_msgs::msg::Trajectory msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::skidpad_msgs::msg::Trajectory>()
{
  return skidpad_msgs::msg::builder::Init_Trajectory_header();
}

}  // namespace skidpad_msgs

#endif  // SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY__BUILDER_HPP_
