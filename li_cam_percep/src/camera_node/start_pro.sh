#!/bin/bash


source install/setup.bash
gnome-terminal -x bash -c "ros2 run camera_node camera_node_1_pro"

source install/setup.bash
gnome-terminal -x bash -c "ros2 run camera_node camera_node_2_pro"


source install/setup.bash
gnome-terminal -x bash -c "ros2 run rqt_image_view rqt_image_view"


 


