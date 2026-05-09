cmake_minimum_required(VERSION 3.5)
project(skidpad_planner)

set(CMAKE_CXX_STANDARD 17)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(skidpad_msgs REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(
  include
  ${EIGEN3_INCLUDE_DIR}
)

add_executable(skidpad_planner_node
  src/main.cpp
  src/path_generator.cpp
  src/path_generator_handle.cpp
  src/Track/track_base.cpp
  src/Track/skidpad_track.cpp
  src/Utils/visual.cpp
)

ament_target_dependencies(skidpad_planner_node
  rclcpp
  std_msgs
  geometry_msgs
  visualization_msgs
  skidpad_msgs
)

target_link_libraries(skidpad_planner_node
  ${OpenCV_LIBS}
)

install(TARGETS skidpad_planner_node
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY config launch
  DESTINATION share/${PROJECT_NAME})

ament_package()