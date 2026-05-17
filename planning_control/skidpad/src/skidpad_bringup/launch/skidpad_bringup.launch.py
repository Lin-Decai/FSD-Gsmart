import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    mode_arg = DeclareLaunchArgument(
        'mode', default_value='simulation',
        description='simulation 或 real_vehicle'
    )
    mode = LaunchConfiguration('mode')

    is_sim = PythonExpression(["'", mode, "' == 'simulation'"])

    # ---------- 各包 config 路径 ----------
    lidar_dir   = get_package_share_directory('lidar_cone_detector')
    icp_dir     = get_package_share_directory('skidpad_icp')
    planner_dir = get_package_share_directory('skidpad_planner')
    control_dir = get_package_share_directory('skidpad_control')

    # ---------- 仿真专用: FSDS ROS 桥接节点 ----------
    # 发布: /lidar_points (原始雷达), /estimation/slam/state, /go_cmd
    # 订阅: /control_command
    fsds_bridge = Node(
        package='fsds_ros_bridge',
        executable='fsds_bridge_node',
        name='fsds_bridge',
        output='screen',
        condition=IfCondition(is_sim),
    )

    # ---------- 雷达锥桶检测 ----------
    # 订阅 /lidar_points (PointCloud2), 发布 /hesai/cone_positions (PoseArray)
    lidar_cone_detector = Node(
        package='lidar_cone_detector',
        executable='lidar_cone_detector',
        name='hesai_processor',
        output='screen',
        parameters=[os.path.join(lidar_dir, 'config', 'lidar_cone_detector.yaml')],
    )

    # ---------- ICP 节点 ----------
    # 订阅 /hesai/cone_positions (PoseArray), 发布 /transform_matrix
    icp_node = Node(
        package='skidpad_icp',
        executable='skidpad_detector_node',
        name='skidpad_detector_node',
        output='screen',
        parameters=[os.path.join(icp_dir, 'config', 'skidpad_detector.yaml')],
    )

    # ---------- 路径规划节点 ----------
    planner_node = Node(
        package='skidpad_planner',
        executable='skidpad_planner_node',
        name='path_generator',
        output='screen',
        parameters=[
            os.path.join(planner_dir, 'config', 'skidpad_planner.yaml'),
            {'simulation': PythonExpression([
                "True if '", mode, "' == 'simulation' else False"
            ])},
        ],
    )

    # ---------- 控制节点 ----------
    control_node = Node(
        package='skidpad_control',
        executable='pure_pursuit_node',
        name='pure_pursuit_controller',
        output='screen',
        parameters=[
            os.path.join(control_dir, 'config', 'pure_pursuit_params.yaml'),
            {'mode': mode},
        ],
    )

    return LaunchDescription([
        mode_arg,
        fsds_bridge,
        lidar_cone_detector,
        icp_node,
        planner_node,
        control_node,
    ])
