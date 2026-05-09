// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from yolo_msgs:msg/DetectedObject.idl
// generated code does not contain a copyright notice

#ifndef YOLO_MSGS__MSG__DETAIL__DETECTED_OBJECT__BUILDER_HPP_
#define YOLO_MSGS__MSG__DETAIL__DETECTED_OBJECT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "yolo_msgs/msg/detail/detected_object__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace yolo_msgs
{

namespace msg
{

namespace builder
{

class Init_DetectedObject_pose
{
public:
  explicit Init_DetectedObject_pose(::yolo_msgs::msg::DetectedObject & msg)
  : msg_(msg)
  {}
  ::yolo_msgs::msg::DetectedObject pose(::yolo_msgs::msg::DetectedObject::_pose_type arg)
  {
    msg_.pose = std::move(arg);
    return std::move(msg_);
  }

private:
  ::yolo_msgs::msg::DetectedObject msg_;
};

class Init_DetectedObject_class_id
{
public:
  explicit Init_DetectedObject_class_id(::yolo_msgs::msg::DetectedObject & msg)
  : msg_(msg)
  {}
  Init_DetectedObject_pose class_id(::yolo_msgs::msg::DetectedObject::_class_id_type arg)
  {
    msg_.class_id = std::move(arg);
    return Init_DetectedObject_pose(msg_);
  }

private:
  ::yolo_msgs::msg::DetectedObject msg_;
};

class Init_DetectedObject_class_name
{
public:
  Init_DetectedObject_class_name()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_DetectedObject_class_id class_name(::yolo_msgs::msg::DetectedObject::_class_name_type arg)
  {
    msg_.class_name = std::move(arg);
    return Init_DetectedObject_class_id(msg_);
  }

private:
  ::yolo_msgs::msg::DetectedObject msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::yolo_msgs::msg::DetectedObject>()
{
  return yolo_msgs::msg::builder::Init_DetectedObject_class_name();
}

}  // namespace yolo_msgs

#endif  // YOLO_MSGS__MSG__DETAIL__DETECTED_OBJECT__BUILDER_HPP_
