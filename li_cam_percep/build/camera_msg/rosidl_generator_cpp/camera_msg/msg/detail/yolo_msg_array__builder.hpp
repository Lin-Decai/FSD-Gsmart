// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from camera_msg:msg/YoloMsgArray.idl
// generated code does not contain a copyright notice

#ifndef CAMERA_MSG__MSG__DETAIL__YOLO_MSG_ARRAY__BUILDER_HPP_
#define CAMERA_MSG__MSG__DETAIL__YOLO_MSG_ARRAY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "camera_msg/msg/detail/yolo_msg_array__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace camera_msg
{

namespace msg
{

namespace builder
{

class Init_YoloMsgArray_cones
{
public:
  explicit Init_YoloMsgArray_cones(::camera_msg::msg::YoloMsgArray & msg)
  : msg_(msg)
  {}
  ::camera_msg::msg::YoloMsgArray cones(::camera_msg::msg::YoloMsgArray::_cones_type arg)
  {
    msg_.cones = std::move(arg);
    return std::move(msg_);
  }

private:
  ::camera_msg::msg::YoloMsgArray msg_;
};

class Init_YoloMsgArray_header
{
public:
  Init_YoloMsgArray_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_YoloMsgArray_cones header(::camera_msg::msg::YoloMsgArray::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_YoloMsgArray_cones(msg_);
  }

private:
  ::camera_msg::msg::YoloMsgArray msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::camera_msg::msg::YoloMsgArray>()
{
  return camera_msg::msg::builder::Init_YoloMsgArray_header();
}

}  // namespace camera_msg

#endif  // CAMERA_MSG__MSG__DETAIL__YOLO_MSG_ARRAY__BUILDER_HPP_
