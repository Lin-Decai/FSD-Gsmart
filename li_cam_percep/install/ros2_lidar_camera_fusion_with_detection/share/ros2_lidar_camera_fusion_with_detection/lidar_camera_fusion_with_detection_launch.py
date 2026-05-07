from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    sort1=Node(package="ros2_lidar_camera_fusion_with_detection",executable="lidar_camera_fusion_with_detection1",name="sort_1")
    sort2=Node(package="ros2_lidar_camera_fusion_with_detection",executable="lidar_camera_fusion_with_detection2",name="sort_2")
    dwf=Node(package="ros2_lidar_camera_fusion_with_detection",executable="multi_sensor_fusion_node",name="DWF")

    return LaunchDescription([sort1,sort2,dwf])