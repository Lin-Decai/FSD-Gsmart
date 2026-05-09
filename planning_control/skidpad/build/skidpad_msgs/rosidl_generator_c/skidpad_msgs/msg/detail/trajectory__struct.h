// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from skidpad_msgs:msg/Trajectory.idl
// generated code does not contain a copyright notice

#ifndef SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY__STRUCT_H_
#define SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY__STRUCT_H_

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
// Member 'trajectory'
#include "skidpad_msgs/msg/detail/trajectory_point__struct.h"
// Member 'remaining_distance'
#include "std_msgs/msg/detail/float64__struct.h"

/// Struct defined in msg/Trajectory in the package skidpad_msgs.
/**
  * 轨迹消息，由一系列轨迹点组成
 */
typedef struct skidpad_msgs__msg__Trajectory
{
  std_msgs__msg__Header header;
  /// 轨迹点数组
  skidpad_msgs__msg__TrajectoryPoint__Sequence trajectory;
  /// 当前车辆位置到全局轨迹终点的弧长距离（米）
  std_msgs__msg__Float64 remaining_distance;
} skidpad_msgs__msg__Trajectory;

// Struct for a sequence of skidpad_msgs__msg__Trajectory.
typedef struct skidpad_msgs__msg__Trajectory__Sequence
{
  skidpad_msgs__msg__Trajectory * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} skidpad_msgs__msg__Trajectory__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY__STRUCT_H_
