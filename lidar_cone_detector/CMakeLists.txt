set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Release)

cmake_minimum_required(VERSION 3.5)
project(lidar_cone_detector)

# Default to C++14
  set(CMAKE_CXX_STANDARD 14)


# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(pcl_ros REQUIRED)
# 启用 CMP0074 策略
cmake_policy(SET CMP0074 NEW)

# PCL components
find_package(PCL 1.10.0 REQUIRED
  COMPONENTS common filters segmentation kdtree search)

# Include directories
include_directories(
  ${PCL_INCLUDE_DIRS}
  ${pcl_conversions_INCLUDE_DIRS}
)

# Create executable
add_executable(lidar_cone_detector src/lidar_cone_detector_3.19.cpp)

# Link libraries
target_link_libraries(lidar_cone_detector
  ${rclcpp_LIBRARIES}
  ${sensor_msgs_LIBRARIES}
  ${geometry_msgs_LIBRARIES}
  ${pcl_conversions_LIBRARIES}
  ${PCL_LIBRARIES}
)

# Install targets
install(TARGETS lidar_cone_detector
  DESTINATION lib/${PROJECT_NAME})

# Export dependencies
ament_export_dependencies(
  rclcpp
  sensor_msgs
  geometry_msgs
  pcl_conversions
  pcl_ros
)

ament_package()

