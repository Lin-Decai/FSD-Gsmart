// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from skidpad_msgs:msg/ControlCommand.idl
// generated code does not contain a copyright notice

#ifndef SKIDPAD_MSGS__MSG__DETAIL__CONTROL_COMMAND__BUILDER_HPP_
#define SKIDPAD_MSGS__MSG__DETAIL__CONTROL_COMMAND__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "skidpad_msgs/msg/detail/control_command__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace skidpad_msgs
{

namespace msg
{

namespace builder
{

class Init_ControlCommand_steering_angle
{
public:
  explicit Init_ControlCommand_steering_angle(::skidpad_msgs::msg::ControlCommand & msg)
  : msg_(msg)
  {}
  ::skidpad_msgs::msg::ControlCommand steering_angle(::skidpad_msgs::msg::ControlCommand::_steering_angle_type arg)
  {
    msg_.steering_angle = std::move(arg);
    return std::move(msg_);
  }

private:
  ::skidpad_msgs::msg::ControlCommand msg_;
};

class Init_ControlCommand_throttle
{
public:
  explicit Init_ControlCommand_throttle(::skidpad_msgs::msg::ControlCommand & msg)
  : msg_(msg)
  {}
  Init_ControlCommand_steering_angle throttle(::skidpad_msgs::msg::ControlCommand::_throttle_type arg)
  {
    msg_.throttle = std::move(arg);
    return Init_ControlCommand_steering_angle(msg_);
  }

private:
  ::skidpad_msgs::msg::ControlCommand msg_;
};

class Init_ControlCommand_header
{
public:
  Init_ControlCommand_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ControlCommand_throttle header(::skidpad_msgs::msg::ControlCommand::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_ControlCommand_throttle(msg_);
  }

private:
  ::skidpad_msgs::msg::ControlCommand msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::skidpad_msgs::msg::ControlCommand>()
{
  return skidpad_msgs::msg::builder::Init_ControlCommand_header();
}

}  // namespace skidpad_msgs

#endif  // SKIDPAD_MSGS__MSG__DETAIL__CONTROL_COMMAND__BUILDER_HPP_
