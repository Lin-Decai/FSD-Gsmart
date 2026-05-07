// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from yolo_msgs:msg/DetectedObject.idl
// generated code does not contain a copyright notice

#ifndef YOLO_MSGS__MSG__DETAIL__DETECTED_OBJECT__STRUCT_H_
#define YOLO_MSGS__MSG__DETAIL__DETECTED_OBJECT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'class_name'
#include "rosidl_runtime_c/string.h"
// Member 'pose'
#include "geometry_msgs/msg/detail/pose__struct.h"

/// Struct defined in msg/DetectedObject in the package yolo_msgs.
typedef struct yolo_msgs__msg__DetectedObject
{
  /// 目标类别（如"car"、"person"）
  rosidl_runtime_c__String class_name;
  /// 目标类别ID（数字标识，如0代表car）
  int32_t class_id;
  /// 目标的姿态（位置+朝向）
  geometry_msgs__msg__Pose pose;
} yolo_msgs__msg__DetectedObject;

// Struct for a sequence of yolo_msgs__msg__DetectedObject.
typedef struct yolo_msgs__msg__DetectedObject__Sequence
{
  yolo_msgs__msg__DetectedObject * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} yolo_msgs__msg__DetectedObject__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // YOLO_MSGS__MSG__DETAIL__DETECTED_OBJECT__STRUCT_H_
