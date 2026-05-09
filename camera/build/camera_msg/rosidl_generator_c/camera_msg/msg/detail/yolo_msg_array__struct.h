// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from camera_msg:msg/YoloMsgArray.idl
// generated code does not contain a copyright notice

#ifndef CAMERA_MSG__MSG__DETAIL__YOLO_MSG_ARRAY__STRUCT_H_
#define CAMERA_MSG__MSG__DETAIL__YOLO_MSG_ARRAY__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'cones'
#include "camera_msg/msg/detail/yolo_msg__struct.h"

/// Struct defined in msg/YoloMsgArray in the package camera_msg.
typedef struct camera_msg__msg__YoloMsgArray
{
  std_msgs__msg__Header header;
  camera_msg__msg__YoloMsg__Sequence cones;
} camera_msg__msg__YoloMsgArray;

// Struct for a sequence of camera_msg__msg__YoloMsgArray.
typedef struct camera_msg__msg__YoloMsgArray__Sequence
{
  camera_msg__msg__YoloMsgArray * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} camera_msg__msg__YoloMsgArray__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CAMERA_MSG__MSG__DETAIL__YOLO_MSG_ARRAY__STRUCT_H_
