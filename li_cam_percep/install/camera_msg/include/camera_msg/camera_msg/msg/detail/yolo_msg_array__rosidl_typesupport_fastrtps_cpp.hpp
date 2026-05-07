// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from camera_msg:msg/YoloMsgArray.idl
// generated code does not contain a copyright notice

#ifndef CAMERA_MSG__MSG__DETAIL__YOLO_MSG_ARRAY__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define CAMERA_MSG__MSG__DETAIL__YOLO_MSG_ARRAY__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "camera_msg/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "camera_msg/msg/detail/yolo_msg_array__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace camera_msg
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_camera_msg
cdr_serialize(
  const camera_msg::msg::YoloMsgArray & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_camera_msg
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  camera_msg::msg::YoloMsgArray & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_camera_msg
get_serialized_size(
  const camera_msg::msg::YoloMsgArray & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_camera_msg
max_serialized_size_YoloMsgArray(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace camera_msg

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_camera_msg
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, camera_msg, msg, YoloMsgArray)();

#ifdef __cplusplus
}
#endif

#endif  // CAMERA_MSG__MSG__DETAIL__YOLO_MSG_ARRAY__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
