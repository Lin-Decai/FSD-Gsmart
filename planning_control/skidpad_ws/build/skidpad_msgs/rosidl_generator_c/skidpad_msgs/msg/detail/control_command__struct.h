// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from skidpad_msgs:msg/ControlCommand.idl
// generated code does not contain a copyright notice

#ifndef SKIDPAD_MSGS__MSG__DETAIL__CONTROL_COMMAND__STRUCT_H_
#define SKIDPAD_MSGS__MSG__DETAIL__CONTROL_COMMAND__STRUCT_H_

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
// Member 'throttle'
// Member 'steering_angle'
#include "std_msgs/msg/detail/float32__struct.h"

/// Struct defined in msg/ControlCommand in the package skidpad_msgs.
/**
  * 控制命令
 */
typedef struct skidpad_msgs__msg__ControlCommand
{
  std_msgs__msg__Header header;
  /// 油门值，范围[-1, 1]，正为加速，负为制动
  std_msgs__msg__Float32 throttle;
  /// 转向角，范围[-1, 1]，左负右正
  std_msgs__msg__Float32 steering_angle;
} skidpad_msgs__msg__ControlCommand;

// Struct for a sequence of skidpad_msgs__msg__ControlCommand.
typedef struct skidpad_msgs__msg__ControlCommand__Sequence
{
  skidpad_msgs__msg__ControlCommand * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} skidpad_msgs__msg__ControlCommand__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SKIDPAD_MSGS__MSG__DETAIL__CONTROL_COMMAND__STRUCT_H_
