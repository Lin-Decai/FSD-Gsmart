// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from skidpad_msgs:msg/CarStateDt.idl
// generated code does not contain a copyright notice

#ifndef SKIDPAD_MSGS__MSG__DETAIL__CAR_STATE_DT__STRUCT_H_
#define SKIDPAD_MSGS__MSG__DETAIL__CAR_STATE_DT__STRUCT_H_

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
// Member 'car_state_dt'
// Member 'car_state_a'
#include "geometry_msgs/msg/detail/pose2_d__struct.h"

/// Struct defined in msg/CarStateDt in the package skidpad_msgs.
/**
  * 车辆状态的时间导数
 */
typedef struct skidpad_msgs__msg__CarStateDt
{
  std_msgs__msg__Header header;
  /// 速度 (x, y, theta)
  geometry_msgs__msg__Pose2D car_state_dt;
  /// 加速度 (x, y, theta)
  geometry_msgs__msg__Pose2D car_state_a;
} skidpad_msgs__msg__CarStateDt;

// Struct for a sequence of skidpad_msgs__msg__CarStateDt.
typedef struct skidpad_msgs__msg__CarStateDt__Sequence
{
  skidpad_msgs__msg__CarStateDt * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} skidpad_msgs__msg__CarStateDt__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SKIDPAD_MSGS__MSG__DETAIL__CAR_STATE_DT__STRUCT_H_
