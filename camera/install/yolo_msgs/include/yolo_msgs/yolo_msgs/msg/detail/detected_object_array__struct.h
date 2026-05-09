// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from yolo_msgs:msg/DetectedObjectArray.idl
// generated code does not contain a copyright notice

#ifndef YOLO_MSGS__MSG__DETAIL__DETECTED_OBJECT_ARRAY__STRUCT_H_
#define YOLO_MSGS__MSG__DETAIL__DETECTED_OBJECT_ARRAY__STRUCT_H_

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
// Member 'objects'
#include "yolo_msgs/msg/detail/detected_object__struct.h"

/// Struct defined in msg/DetectedObjectArray in the package yolo_msgs.
typedef struct yolo_msgs__msg__DetectedObjectArray
{
  std_msgs__msg__Header header;
  /// 检测目标数组
  yolo_msgs__msg__DetectedObject__Sequence objects;
} yolo_msgs__msg__DetectedObjectArray;

// Struct for a sequence of yolo_msgs__msg__DetectedObjectArray.
typedef struct yolo_msgs__msg__DetectedObjectArray__Sequence
{
  yolo_msgs__msg__DetectedObjectArray * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} yolo_msgs__msg__DetectedObjectArray__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // YOLO_MSGS__MSG__DETAIL__DETECTED_OBJECT_ARRAY__STRUCT_H_
