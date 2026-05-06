from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    pkg_dir = get_package_share_directory('skidpad_control')
    param_file = os.path.join(pkg_dir, 'config', 'pure_pursuit_params.yaml')

    return LaunchDescription([
        Node(
            package='skidpad_control',
            executable='pure_pursuit_node',
            name='pure_pursuit_controller',
            output='screen',
            parameters=[param_file],
            # 可在 launch 时通过命令行参数覆盖 mode
            arguments=['--ros-args', '--params-file', param_file]
        )
    ])