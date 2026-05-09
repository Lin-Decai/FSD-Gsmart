from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    pkg_dir = get_package_share_directory('icp_test_tools')
    config = os.path.join(pkg_dir, 'config', 'test_params.yaml')

    return LaunchDescription([
        Node(
            package='icp_test_tools',
            executable='publisher_node',
            name='test_lidar_publisher',
            output='screen',
            parameters=[config]
        ),
        Node(
            package='icp_test_tools',
            executable='printer_node',
            name='transform_printer',
            output='screen'
        ),
        # 同时启动你的 skidpad_icp 节点（确保参数指向正确的地图文件）
        Node(
            package='skidpad_icp',
            executable='skidpad_detector_node',
            name='skidpad_detector_node',
            output='screen',
            parameters=[os.path.join(
                get_package_share_directory('skidpad_icp'),
                'config',
                'skidpad_detector.yaml'
            )]
        )
    ])