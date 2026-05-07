// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from camera_msg:msg/YoloMsg.idl
// generated code does not contain a copyright notice

#ifndef CAMERA_MSG__MSG__DETAIL__YOLO_MSG__STRUCT_H_
#define CAMERA_MSG__MSG__DETAIL__YOLO_MSG__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/YoloMsg in the package camera_msg.
typedef struct camera_msg__msg__YoloMsg
{
  uint32_t xmin;
  uint32_t ymin;
  uint32_t xmax;
  uint32_t ymax;
  float label;
  float confidence;
  float zmin;
  float zmax;
} camera_msg__msg__YoloMsg;

// Struct for a sequence of camera_msg__msg__YoloMsg.
typedef struct camera_msg__msg__YoloMsg__Sequence
{
  camera_msg__msg__YoloMsg * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} camera_msg__msg__YoloMsg__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CAMERA_MSG__MSG__DETAIL__YOLO_MSG__STRUCT_H_
