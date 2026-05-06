from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    pkg_dir = get_package_share_directory('skidpad_planner')
    config_file = os.path.join(pkg_dir, 'config', 'skidpad_planner.yaml')

    return LaunchDescription([
        Node(
            package='skidpad_planner',
            executable='skidpad_planner_node',
            name='path_generator',
            output='screen',
            parameters=[config_file]
        )
    ])

