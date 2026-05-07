#!/bin/bash

# 检查输入参数
if [ "$1" = "ROS1" ]; then
    rm src/pbox_node/CMakeLists.txt src/pbox_node/package.xml
    cp src/pbox_node/CMakeLists_ros1.txt src/pbox_node/CMakeLists.txt
    cp src/pbox_node/package_ros1.xml src/pbox_node/package.xml
    echo "Configured ROS1 CMakeLists.txt and package.xml"
elif [ "$1" = "ROS2" ]; then
    rm src/pbox_node/CMakeLists.txt src/pbox_node/package.xml
    cp src/pbox_node/CMakeLists_ros2.txt src/pbox_node/CMakeLists.txt
    cp src/pbox_node/package_ros2.xml src/pbox_node/package.xml
    echo "Configured ROS2 CMakeLists.txt and package.xml"
else
    echo "Usage: $0 [ROS1|ROS2]"
    exit 1
fi