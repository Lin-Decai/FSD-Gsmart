// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from camera_msg:msg/YoloMsgArray.idl
// generated code does not contain a copyright notice
#include "camera_msg/msg/detail/yolo_msg_array__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `cones`
#include "camera_msg/msg/detail/yolo_msg__functions.h"

bool
camera_msg__msg__YoloMsgArray__init(camera_msg__msg__YoloMsgArray * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    camera_msg__msg__YoloMsgArray__fini(msg);
    return false;
  }
  // cones
  if (!camera_msg__msg__YoloMsg__Sequence__init(&msg->cones, 0)) {
    camera_msg__msg__YoloMsgArray__fini(msg);
    return false;
  }
  return true;
}

void
camera_msg__msg__YoloMsgArray__fini(camera_msg__msg__YoloMsgArray * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // cones
  camera_msg__msg__YoloMsg__Sequence__fini(&msg->cones);
}

bool
camera_msg__msg__YoloMsgArray__are_equal(const camera_msg__msg__YoloMsgArray * lhs, const camera_msg__msg__YoloMsgArray * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // cones
  if (!camera_msg__msg__YoloMsg__Sequence__are_equal(
      &(lhs->cones), &(rhs->cones)))
  {
    return false;
  }
  return true;
}

bool
camera_msg__msg__YoloMsgArray__copy(
  const camera_msg__msg__YoloMsgArray * input,
  camera_msg__msg__YoloMsgArray * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // cones
  if (!camera_msg__msg__YoloMsg__Sequence__copy(
      &(input->cones), &(output->cones)))
  {
    return false;
  }
  return true;
}

camera_msg__msg__YoloMsgArray *
camera_msg__msg__YoloMsgArray__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  camera_msg__msg__YoloMsgArray * msg = (camera_msg__msg__YoloMsgArray *)allocator.allocate(sizeof(camera_msg__msg__YoloMsgArray), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(camera_msg__msg__YoloMsgArray));
  bool success = camera_msg__msg__YoloMsgArray__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
camera_msg__msg__YoloMsgArray__destroy(camera_msg__msg__YoloMsgArray * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    camera_msg__msg__YoloMsgArray__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
camera_msg__msg__YoloMsgArray__Sequence__init(camera_msg__msg__YoloMsgArray__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  camera_msg__msg__YoloMsgArray * data = NULL;

  if (size) {
    data = (camera_msg__msg__YoloMsgArray *)allocator.zero_allocate(size, sizeof(camera_msg__msg__YoloMsgArray), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = camera_msg__msg__YoloMsgArray__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        camera_msg__msg__YoloMsgArray__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
camera_msg__msg__YoloMsgArray__Sequence__fini(camera_msg__msg__YoloMsgArray__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      camera_msg__msg__YoloMsgArray__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

camera_msg__msg__YoloMsgArray__Sequence *
camera_msg__msg__YoloMsgArray__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  camera_msg__msg__YoloMsgArray__Sequence * array = (camera_msg__msg__YoloMsgArray__Sequence *)allocator.allocate(sizeof(camera_msg__msg__YoloMsgArray__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = camera_msg__msg__YoloMsgArray__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
camera_msg__msg__YoloMsgArray__Sequence__destroy(camera_msg__msg__YoloMsgArray__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    camera_msg__msg__YoloMsgArray__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
camera_msg__msg__YoloMsgArray__Sequence__are_equal(const camera_msg__msg__YoloMsgArray__Sequence * lhs, const camera_msg__msg__YoloMsgArray__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!camera_msg__msg__YoloMsgArray__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
camera_msg__msg__YoloMsgArray__Sequence__copy(
  const camera_msg__msg__YoloMsgArray__Sequence * input,
  camera_msg__msg__YoloMsgArray__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(camera_msg__msg__YoloMsgArray);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    camera_msg__msg__YoloMsgArray * data =
      (camera_msg__msg__YoloMsgArray *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!camera_msg__msg__YoloMsgArray__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          camera_msg__msg__YoloMsgArray__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!camera_msg__msg__YoloMsgArray__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
