// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from skidpad_msgs:msg/CarState.idl
// generated code does not contain a copyright notice

#ifndef SKIDPAD_MSGS__MSG__DETAIL__CAR_STATE__STRUCT_H_
#define SKIDPAD_MSGS__MSG__DETAIL__CAR_STATE__STRUCT_H_

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
// Member 'car_state'
#include "geometry_msgs/msg/detail/pose2_d__struct.h"
// Member 'car_state_dt'
#include "skidpad_msgs/msg/detail/car_state_dt__struct.h"

/// Struct defined in msg/CarState in the package skidpad_msgs.
/**
  * 车辆状态主消息
 */
typedef struct skidpad_msgs__msg__CarState
{
  std_msgs__msg__Header header;
  /// 位置和姿态角 (x, y, theta)
  geometry_msgs__msg__Pose2D car_state;
  /// 一阶导数信息 (速度、加速度)
  skidpad_msgs__msg__CarStateDt car_state_dt;
} skidpad_msgs__msg__CarState;

// Struct for a sequence of skidpad_msgs__msg__CarState.
typedef struct skidpad_msgs__msg__CarState__Sequence
{
  skidpad_msgs__msg__CarState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} skidpad_msgs__msg__CarState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SKIDPAD_MSGS__MSG__DETAIL__CAR_STATE__STRUCT_H_
