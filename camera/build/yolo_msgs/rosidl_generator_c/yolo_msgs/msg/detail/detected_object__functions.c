// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from yolo_msgs:msg/DetectedObject.idl
// generated code does not contain a copyright notice
#include "yolo_msgs/msg/detail/detected_object__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `class_name`
#include "rosidl_runtime_c/string_functions.h"
// Member `pose`
#include "geometry_msgs/msg/detail/pose__functions.h"

bool
yolo_msgs__msg__DetectedObject__init(yolo_msgs__msg__DetectedObject * msg)
{
  if (!msg) {
    return false;
  }
  // class_name
  if (!rosidl_runtime_c__String__init(&msg->class_name)) {
    yolo_msgs__msg__DetectedObject__fini(msg);
    return false;
  }
  // class_id
  // pose
  if (!geometry_msgs__msg__Pose__init(&msg->pose)) {
    yolo_msgs__msg__DetectedObject__fini(msg);
    return false;
  }
  return true;
}

void
yolo_msgs__msg__DetectedObject__fini(yolo_msgs__msg__DetectedObject * msg)
{
  if (!msg) {
    return;
  }
  // class_name
  rosidl_runtime_c__String__fini(&msg->class_name);
  // class_id
  // pose
  geometry_msgs__msg__Pose__fini(&msg->pose);
}

bool
yolo_msgs__msg__DetectedObject__are_equal(const yolo_msgs__msg__DetectedObject * lhs, const yolo_msgs__msg__DetectedObject * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // class_name
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->class_name), &(rhs->class_name)))
  {
    return false;
  }
  // class_id
  if (lhs->class_id != rhs->class_id) {
    return false;
  }
  // pose
  if (!geometry_msgs__msg__Pose__are_equal(
      &(lhs->pose), &(rhs->pose)))
  {
    return false;
  }
  return true;
}

bool
yolo_msgs__msg__DetectedObject__copy(
  const yolo_msgs__msg__DetectedObject * input,
  yolo_msgs__msg__DetectedObject * output)
{
  if (!input || !output) {
    return false;
  }
  // class_name
  if (!rosidl_runtime_c__String__copy(
      &(input->class_name), &(output->class_name)))
  {
    return false;
  }
  // class_id
  output->class_id = input->class_id;
  // pose
  if (!geometry_msgs__msg__Pose__copy(
      &(input->pose), &(output->pose)))
  {
    return false;
  }
  return true;
}

yolo_msgs__msg__DetectedObject *
yolo_msgs__msg__DetectedObject__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  yolo_msgs__msg__DetectedObject * msg = (yolo_msgs__msg__DetectedObject *)allocator.allocate(sizeof(yolo_msgs__msg__DetectedObject), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(yolo_msgs__msg__DetectedObject));
  bool success = yolo_msgs__msg__DetectedObject__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
yolo_msgs__msg__DetectedObject__destroy(yolo_msgs__msg__DetectedObject * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    yolo_msgs__msg__DetectedObject__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
yolo_msgs__msg__DetectedObject__Sequence__init(yolo_msgs__msg__DetectedObject__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  yolo_msgs__msg__DetectedObject * data = NULL;

  if (size) {
    data = (yolo_msgs__msg__DetectedObject *)allocator.zero_allocate(size, sizeof(yolo_msgs__msg__DetectedObject), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = yolo_msgs__msg__DetectedObject__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        yolo_msgs__msg__DetectedObject__fini(&data[i - 1]);
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
yolo_msgs__msg__DetectedObject__Sequence__fini(yolo_msgs__msg__DetectedObject__Sequence * array)
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
      yolo_msgs__msg__DetectedObject__fini(&array->data[i]);
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

yolo_msgs__msg__DetectedObject__Sequence *
yolo_msgs__msg__DetectedObject__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  yolo_msgs__msg__DetectedObject__Sequence * array = (yolo_msgs__msg__DetectedObject__Sequence *)allocator.allocate(sizeof(yolo_msgs__msg__DetectedObject__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = yolo_msgs__msg__DetectedObject__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
yolo_msgs__msg__DetectedObject__Sequence__destroy(yolo_msgs__msg__DetectedObject__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    yolo_msgs__msg__DetectedObject__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
yolo_msgs__msg__DetectedObject__Sequence__are_equal(const yolo_msgs__msg__DetectedObject__Sequence * lhs, const yolo_msgs__msg__DetectedObject__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!yolo_msgs__msg__DetectedObject__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
yolo_msgs__msg__DetectedObject__Sequence__copy(
  const yolo_msgs__msg__DetectedObject__Sequence * input,
  yolo_msgs__msg__DetectedObject__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(yolo_msgs__msg__DetectedObject);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    yolo_msgs__msg__DetectedObject * data =
      (yolo_msgs__msg__DetectedObject *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!yolo_msgs__msg__DetectedObject__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          yolo_msgs__msg__DetectedObject__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!yolo_msgs__msg__DetectedObject__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
