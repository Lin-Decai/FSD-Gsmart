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
        description='运行模式: simulation 或 real_vehicle'
    )
    mode = LaunchConfiguration('mode')

    is_sim = PythonExpression(["'", mode, "' == 'simulation'"])

    # ---------- 各包路径 ----------
    icp_dir     = get_package_share_directory('skidpad_icp')
    planner_dir = get_package_share_directory('skidpad_planner')
    control_dir = get_package_share_directory('skidpad_control')

    # ---------- 仿真专用: FSDS 桥接节点 ----------
    # 发布 CarState (/estimation/slam/state)、锥桶 (/perception/lidar_cluster)、
    # GO 信号 (/go_cmd)，订阅 ControlCommand (/control_command)
    fsds_bridge = Node(
        package='fsds_bridge',
        executable='fsds_bridge_node',
        name='fsds_bridge',
        output='screen',
        condition=IfCondition(is_sim),
    )

    # ---------- ICP 节点 ----------
    # 仿真: 锥桶从 fsds_bridge 来; 实车: 从真实感知来
    icp_node = Node(
        package='skidpad_icp',
        executable='skidpad_detector_node',
        name='skidpad_detector_node',
        output='screen',
        parameters=[os.path.join(icp_dir, 'config', 'skidpad_detector.yaml')],
    )

    # ---------- 路径规划节点 ----------
    # 仿真和实车统一订阅 /estimation/slam/state
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
    # 仿真和实车统一:
    #   订阅 /estimation/slam/state (CarState)
    #   订阅 /go_cmd (Bool)
    #   发布 /control_command (ControlCommand)
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
        icp_node,
        planner_node,
        control_node,
    ])
