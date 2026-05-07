// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from camera_msg:msg/YoloMsg.idl
// generated code does not contain a copyright notice

#ifndef CAMERA_MSG__MSG__DETAIL__YOLO_MSG__FUNCTIONS_H_
#define CAMERA_MSG__MSG__DETAIL__YOLO_MSG__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "camera_msg/msg/rosidl_generator_c__visibility_control.h"

#include "camera_msg/msg/detail/yolo_msg__struct.h"

/// Initialize msg/YoloMsg message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * camera_msg__msg__YoloMsg
 * )) before or use
 * camera_msg__msg__YoloMsg__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_camera_msg
bool
camera_msg__msg__YoloMsg__init(camera_msg__msg__YoloMsg * msg);

/// Finalize msg/YoloMsg message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_camera_msg
void
camera_msg__msg__YoloMsg__fini(camera_msg__msg__YoloMsg * msg);

/// Create msg/YoloMsg message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * camera_msg__msg__YoloMsg__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_camera_msg
camera_msg__msg__YoloMsg *
camera_msg__msg__YoloMsg__create();

/// Destroy msg/YoloMsg message.
/**
 * It calls
 * camera_msg__msg__YoloMsg__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_camera_msg
void
camera_msg__msg__YoloMsg__destroy(camera_msg__msg__YoloMsg * msg);

/// Check for msg/YoloMsg message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_camera_msg
bool
camera_msg__msg__YoloMsg__are_equal(const camera_msg__msg__YoloMsg * lhs, const camera_msg__msg__YoloMsg * rhs);

/// Copy a msg/YoloMsg message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_camera_msg
bool
camera_msg__msg__YoloMsg__copy(
  const camera_msg__msg__YoloMsg * input,
  camera_msg__msg__YoloMsg * output);

/// Initialize array of msg/YoloMsg messages.
/**
 * It allocates the memory for the number of elements and calls
 * camera_msg__msg__YoloMsg__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_camera_msg
bool
camera_msg__msg__YoloMsg__Sequence__init(camera_msg__msg__YoloMsg__Sequence * array, size_t size);

/// Finalize array of msg/YoloMsg messages.
/**
 * It calls
 * camera_msg__msg__YoloMsg__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_camera_msg
void
camera_msg__msg__YoloMsg__Sequence__fini(camera_msg__msg__YoloMsg__Sequence * array);

/// Create array of msg/YoloMsg messages.
/**
 * It allocates the memory for the array and calls
 * camera_msg__msg__YoloMsg__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_camera_msg
camera_msg__msg__YoloMsg__Sequence *
camera_msg__msg__YoloMsg__Sequence__create(size_t size);

/// Destroy array of msg/YoloMsg messages.
/**
 * It calls
 * camera_msg__msg__YoloMsg__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_camera_msg
void
camera_msg__msg__YoloMsg__Sequence__destroy(camera_msg__msg__YoloMsg__Sequence * array);

/// Check for msg/YoloMsg message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_camera_msg
bool
camera_msg__msg__YoloMsg__Sequence__are_equal(const camera_msg__msg__YoloMsg__Sequence * lhs, const camera_msg__msg__YoloMsg__Sequence * rhs);

/// Copy an array of msg/YoloMsg messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_camera_msg
bool
camera_msg__msg__YoloMsg__Sequence__copy(
  const camera_msg__msg__YoloMsg__Sequence * input,
  camera_msg__msg__YoloMsg__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // CAMERA_MSG__MSG__DETAIL__YOLO_MSG__FUNCTIONS_H_
