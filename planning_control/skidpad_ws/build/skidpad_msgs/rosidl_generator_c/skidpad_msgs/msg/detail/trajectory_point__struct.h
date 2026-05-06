// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from skidpad_msgs:msg/TrajectoryPoint.idl
// generated code does not contain a copyright notice

#ifndef SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY_POINT__STRUCT_H_
#define SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY_POINT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'pts'
#include "geometry_msgs/msg/detail/point__struct.h"
// Member 'yaw'
// Member 'curvature'
// Member 'velocity'
// Member 'r'
// Member 'acc'
#include "std_msgs/msg/detail/float64__struct.h"

/// Struct defined in msg/TrajectoryPoint in the package skidpad_msgs.
/**
  * 单个轨迹点信息（车体坐标系）
 */
typedef struct skidpad_msgs__msg__TrajectoryPoint
{
  /// 坐标 (x 向前, y 向左)
  geometry_msgs__msg__Point pts;
  /// 期望航向角
  std_msgs__msg__Float64 yaw;
  /// 路径曲率
  std_msgs__msg__Float64 curvature;
  /// 期望速度
  std_msgs__msg__Float64 velocity;
  /// 横摆角速度（预留）
  std_msgs__msg__Float64 r;
  /// 期望加速度（预留）
  std_msgs__msg__Float64 acc;
} skidpad_msgs__msg__TrajectoryPoint;

// Struct for a sequence of skidpad_msgs__msg__TrajectoryPoint.
typedef struct skidpad_msgs__msg__TrajectoryPoint__Sequence
{
  skidpad_msgs__msg__TrajectoryPoint * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} skidpad_msgs__msg__TrajectoryPoint__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SKIDPAD_MSGS__MSG__DETAIL__TRAJECTORY_POINT__STRUCT_H_
