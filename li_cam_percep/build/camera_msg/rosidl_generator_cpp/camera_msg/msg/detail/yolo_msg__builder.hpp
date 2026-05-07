// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from camera_msg:msg/YoloMsg.idl
// generated code does not contain a copyright notice

#ifndef CAMERA_MSG__MSG__DETAIL__YOLO_MSG__BUILDER_HPP_
#define CAMERA_MSG__MSG__DETAIL__YOLO_MSG__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "camera_msg/msg/detail/yolo_msg__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace camera_msg
{

namespace msg
{

namespace builder
{

class Init_YoloMsg_zmax
{
public:
  explicit Init_YoloMsg_zmax(::camera_msg::msg::YoloMsg & msg)
  : msg_(msg)
  {}
  ::camera_msg::msg::YoloMsg zmax(::camera_msg::msg::YoloMsg::_zmax_type arg)
  {
    msg_.zmax = std::move(arg);
    return std::move(msg_);
  }

private:
  ::camera_msg::msg::YoloMsg msg_;
};

class Init_YoloMsg_zmin
{
public:
  explicit Init_YoloMsg_zmin(::camera_msg::msg::YoloMsg & msg)
  : msg_(msg)
  {}
  Init_YoloMsg_zmax zmin(::camera_msg::msg::YoloMsg::_zmin_type arg)
  {
    msg_.zmin = std::move(arg);
    return Init_YoloMsg_zmax(msg_);
  }

private:
  ::camera_msg::msg::YoloMsg msg_;
};

class Init_YoloMsg_confidence
{
public:
  explicit Init_YoloMsg_confidence(::camera_msg::msg::YoloMsg & msg)
  : msg_(msg)
  {}
  Init_YoloMsg_zmin confidence(::camera_msg::msg::YoloMsg::_confidence_type arg)
  {
    msg_.confidence = std::move(arg);
    return Init_YoloMsg_zmin(msg_);
  }

private:
  ::camera_msg::msg::YoloMsg msg_;
};

class Init_YoloMsg_label
{
public:
  explicit Init_YoloMsg_label(::camera_msg::msg::YoloMsg & msg)
  : msg_(msg)
  {}
  Init_YoloMsg_confidence label(::camera_msg::msg::YoloMsg::_label_type arg)
  {
    msg_.label = std::move(arg);
    return Init_YoloMsg_confidence(msg_);
  }

private:
  ::camera_msg::msg::YoloMsg msg_;
};

class Init_YoloMsg_ymax
{
public:
  explicit Init_YoloMsg_ymax(::camera_msg::msg::YoloMsg & msg)
  : msg_(msg)
  {}
  Init_YoloMsg_label ymax(::camera_msg::msg::YoloMsg::_ymax_type arg)
  {
    msg_.ymax = std::move(arg);
    return Init_YoloMsg_label(msg_);
  }

private:
  ::camera_msg::msg::YoloMsg msg_;
};

class Init_YoloMsg_xmax
{
public:
  explicit Init_YoloMsg_xmax(::camera_msg::msg::YoloMsg & msg)
  : msg_(msg)
  {}
  Init_YoloMsg_ymax xmax(::camera_msg::msg::YoloMsg::_xmax_type arg)
  {
    msg_.xmax = std::move(arg);
    return Init_YoloMsg_ymax(msg_);
  }

private:
  ::camera_msg::msg::YoloMsg msg_;
};

class Init_YoloMsg_ymin
{
public:
  explicit Init_YoloMsg_ymin(::camera_msg::msg::YoloMsg & msg)
  : msg_(msg)
  {}
  Init_YoloMsg_xmax ymin(::camera_msg::msg::YoloMsg::_ymin_type arg)
  {
    msg_.ymin = std::move(arg);
    return Init_YoloMsg_xmax(msg_);
  }

private:
  ::camera_msg::msg::YoloMsg msg_;
};

class Init_YoloMsg_xmin
{
public:
  Init_YoloMsg_xmin()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_YoloMsg_ymin xmin(::camera_msg::msg::YoloMsg::_xmin_type arg)
  {
    msg_.xmin = std::move(arg);
    return Init_YoloMsg_ymin(msg_);
  }

private:
  ::camera_msg::msg::YoloMsg msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::camera_msg::msg::YoloMsg>()
{
  return camera_msg::msg::builder::Init_YoloMsg_xmin();
}

}  // namespace camera_msg

#endif  // CAMERA_MSG__MSG__DETAIL__YOLO_MSG__BUILDER_HPP_
