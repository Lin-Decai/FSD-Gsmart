cmake_minimum_required(VERSION 3.5)
project(skidpad_msgs)

# 查找依赖
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(builtin_interfaces REQUIRED)

# 生成消息
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/CarState.msg"
  "msg/CarStateDt.msg"
  "msg/ControlCommand.msg"
  "msg/Trajectory.msg"
  "msg/TrajectoryPoint.msg"
  DEPENDENCIES std_msgs geometry_msgs
)

ament_export_dependencies(rosidl_default_runtime)

ament_package()