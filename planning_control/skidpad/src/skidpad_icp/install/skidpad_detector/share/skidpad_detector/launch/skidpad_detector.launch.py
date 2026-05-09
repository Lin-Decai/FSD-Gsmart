from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    # 获取包所在目录
    pkg_dir = get_package_share_directory('skidpad_detector')
    # 配置文件路径
    config_file = os.path.join(pkg_dir, 'config', 'skidpad_detector.yaml')

    return LaunchDescription([
        Node(
            package='skidpad_detector',
            executable='skidpad_detector_node',
            name='skidpad_detector_node',
            output='screen',
            parameters=[config_file]  # 加载 YAML 参数文件
        )
    ])