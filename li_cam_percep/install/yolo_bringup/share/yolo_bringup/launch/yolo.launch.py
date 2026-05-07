# Copyright (C) 2023 Miguel Ángel González Santamarta
# 适配两路相机的启动文件，分别使用yolo_node1和yolo_node2节点

from launch import LaunchDescription, LaunchContext
from launch.actions import DeclareLaunchArgument, OpaqueFunction
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node
from launch.conditions import IfCondition


def generate_launch_description():

    def run_two_cameras(context: LaunchContext, use_tracking, use_3d):
        """启动两路相机的节点，分别使用独立的节点文件和参数"""
        use_tracking = eval(context.perform_substitution(use_tracking))
        use_3d = eval(context.perform_substitution(use_3d))

        # -------------------------- 通用参数（两路相机共享） --------------------------
        model_type = LaunchConfiguration("model_type")
        model_type_cmd = DeclareLaunchArgument(
            "model_type",
            default_value="YOLO",
            choices=["YOLO", "World"],
            description="模型类型（YOLO/World）"
        )

        model = LaunchConfiguration("model")
        model_cmd = DeclareLaunchArgument(
            "model",
            default_value=" /home/alexis/dmbc/workspace/camera/src/yolo_ros/best0918.pt",
            description="模型路径"
        )

        tracker = LaunchConfiguration("tracker")
        tracker_cmd = DeclareLaunchArgument(
            "tracker",
            default_value="bytetrack.yaml",
            description="跟踪器配置文件"
        )

        device = LaunchConfiguration("device")
        device_cmd = DeclareLaunchArgument(
            "device",
            default_value="cuda",
            description="运行设备（cpu/cuda）"
        )

        # -------------------------- 相机1专属参数 --------------------------
        # 命名空间（隔离两路相机的话题和节点）
        namespace1 = LaunchConfiguration("namespace1")
        namespace1_cmd = DeclareLaunchArgument(
            "namespace1",
            default_value="yolo_camera1",
            description="相机1节点的命名空间"
        )

        # 输入图像话题（相机1的原始图像）
        input_image_topic1 = LaunchConfiguration("input_image_topic1")
        input_image_topic1_cmd = DeclareLaunchArgument(
            "input_image_topic1",
            default_value="/camera_1/image_raw_1",
            description="相机1的输入图像话题"
        )

        # 深度话题（相机1）
        input_depth_topic1 = LaunchConfiguration("input_depth_topic1")
        input_depth_topic1_cmd = DeclareLaunchArgument(
            "input_depth_topic1",
            default_value="/carla/hero/depth_left/image",
            description="相机1的深度图像话题"
        )

        input_depth_info_topic1 = LaunchConfiguration("input_depth_info_topic1")
        input_depth_info_topic1_cmd = DeclareLaunchArgument(
            "input_depth_info_topic1",
            default_value="/carla/hero/depth_left/camera_info",
            description="相机1的深度信息话题"
        )

        # 检测参数（相机1）
        threshold1 = LaunchConfiguration("threshold1")
        threshold1_cmd = DeclareLaunchArgument(
            "threshold1",
            default_value="0.5",
            description="相机1的检测置信度阈值"
        )

        # 其他参数（与单路类似，添加1后缀区分）
        yolo_encoding1 = LaunchConfiguration("yolo_encoding1")
        yolo_encoding1_cmd = DeclareLaunchArgument("yolo_encoding1", default_value="bgr8")
        enable1 = LaunchConfiguration("enable1")
        enable1_cmd = DeclareLaunchArgument("enable1", default_value="True")
        imgsz_height1 = LaunchConfiguration("imgsz_height1")
        imgsz_height1_cmd = DeclareLaunchArgument("imgsz_height1", default_value="640")
        imgsz_width1 = LaunchConfiguration("imgsz_width1")
        imgsz_width1_cmd = DeclareLaunchArgument("imgsz_width1", default_value="640")
        image_reliability1 = LaunchConfiguration("image_reliability1")
        image_reliability1_cmd = DeclareLaunchArgument("image_reliability1", default_value="1")
        '''
        # -------------------------- 相机2专属参数 --------------------------
        # 命名空间（相机2）
        namespace2 = LaunchConfiguration("namespace2")
        namespace2_cmd = DeclareLaunchArgument(
            "namespace2",
            default_value="yolo_camera2",
            description="相机2节点的命名空间"
        )

        # 输入图像话题（相机2的原始图像）
        input_image_topic2 = LaunchConfiguration("input_image_topic2")
        input_image_topic2_cmd = DeclareLaunchArgument(
            "input_image_topic2",
            default_value="/camera_2/image_raw_2",
            description="相机2的输入图像话题"
        )

        # 深度话题（相机2）
        input_depth_topic2 = LaunchConfiguration("input_depth_topic2")
        input_depth_topic2_cmd = DeclareLaunchArgument(
            "input_depth_topic2",
            default_value="/carla/hero/depth_right/image",
            description="相机2的深度图像话题"
        )

        input_depth_info_topic2 = LaunchConfiguration("input_depth_info_topic2")
        input_depth_info_topic2_cmd = DeclareLaunchArgument(
            "input_depth_info_topic2",
            default_value="/carla/hero/depth_right/camera_info",
            description="相机2的深度信息话题"
        )

        # 检测参数（相机2）
        threshold2 = LaunchConfiguration("threshold2")
        threshold2_cmd = DeclareLaunchArgument(
            "threshold2",
            default_value="0.5",
            description="相机2的检测置信度阈值"
        )

        # 其他参数（与单路类似，添加2后缀区分）
        yolo_encoding2 = LaunchConfiguration("yolo_encoding2")
        yolo_encoding2_cmd = DeclareLaunchArgument("yolo_encoding2", default_value="bgr8")
        enable2 = LaunchConfiguration("enable2")
        enable2_cmd = DeclareLaunchArgument("enable2", default_value="True")
        imgsz_height2 = LaunchConfiguration("imgsz_height2")
        imgsz_height2_cmd = DeclareLaunchArgument("imgsz_height2", default_value="640")
        imgsz_width2 = LaunchConfiguration("imgsz_width2")
        imgsz_width2_cmd = DeclareLaunchArgument("imgsz_width2", default_value="640")
        image_reliability2 = LaunchConfiguration("image_reliability2")
        image_reliability2_cmd = DeclareLaunchArgument("image_reliability2", default_value="1")

        '''

        # -------------------------- 相机1的节点配置（使用yolo_node1） --------------------------
        # 1. YOLO检测节点（相机1专用节点文件）
        yolo_node1 = Node(
            package="yolo_ros",
            executable="yolo_node1",  # 使用复制的yolo-node1.py（编译后为yolo_node1）
            name="yolo_node1",
            namespace=namespace1,
            parameters=[{
                "model_type": model_type,
                "model": model,
                "device": device,
                "yolo_encoding": yolo_encoding1,
                "enable": enable1,
                "threshold": threshold1,
                "iou": 0.7,  # 可根据需要添加独立参数
                "imgsz_height": imgsz_height1,
                "imgsz_width": imgsz_width1,
                "image_reliability": image_reliability1,
                # 其他参数与单路一致，确保使用相机1的参数
            }],
            remappings=[("image_raw", input_image_topic1)],  # 映射到相机1的图像
        )

        # 2. 跟踪节点（相机1）
        tracking_node1 = Node(
            package="yolo_ros",
            executable="tracking_node1",  # 建议也复制跟踪节点并改名，避免冲突
            name="tracking_node1",
            namespace=namespace1,
            parameters=[{"tracker": tracker, "image_reliability": image_reliability1}],
            remappings=[("image_raw", input_image_topic1)],
            condition=IfCondition(PythonExpression([str(use_tracking)])),
        )

        # 3. 调试节点（相机1）
        debug_node1 = Node(
            package="yolo_ros",
            executable="debug_node1",  # 复制调试节点并改名
            name="debug_node1",
            namespace=namespace1,
            parameters=[{"image_reliability": image_reliability1}],
            remappings=[
                ("image_raw", input_image_topic1),
                ("detections", "detections"),  # 话题在命名空间内自动隔离
            ],
        )
        '''
        # -------------------------- 相机2的节点配置（使用yolo_node2） --------------------------
        # 1. YOLO检测节点（相机2专用节点文件）
        yolo_node2 = Node(
            package="yolo_ros",
            executable="yolo_node2",  # 使用复制的yolo-node2.py（编译后为yolo_node2）
            name="yolo_node2",
            namespace=namespace2,
            parameters=[{
                "model_type": model_type,
                "model": model,
                "device": device,
                "yolo_encoding": yolo_encoding2,
                "enable": enable2,
                "threshold": threshold2,
                "iou": 0.7,
                "imgsz_height": imgsz_height2,
                "imgsz_width": imgsz_width2,
                "image_reliability": image_reliability2,
                # 其他参数与相机1独立
            }],
            remappings=[("image_raw", input_image_topic2)],  # 映射到相机2的图像
        )

        # 2. 跟踪节点（相机2）
        tracking_node2 = Node(
            package="yolo_ros",
            executable="tracking_node2",  # 复制跟踪节点并改名
            name="tracking_node2",
            namespace=namespace2,
            parameters=[{"tracker": tracker, "image_reliability": image_reliability2}],
            remappings=[("image_raw", input_image_topic2)],
            condition=IfCondition(PythonExpression([str(use_tracking)])),
        )

        # 3. 调试节点（相机2）
        debug_node2 = Node(
            package="yolo_ros",
            executable="debug_node2",  # 复制调试节点并改名
            name="debug_node2",
            namespace=namespace2,
            parameters=[{"image_reliability": image_reliability2}],
            remappings=[
                ("image_raw", input_image_topic2),
                ("detections", "detections"),
            ],
        )
        '''
        # -------------------------- 汇总所有参数和节点 --------------------------
        return [
            # 通用参数
            model_type_cmd, model_cmd, tracker_cmd, device_cmd,
            # 相机1参数和节点
            namespace1_cmd, input_image_topic1_cmd, input_depth_topic1_cmd,
            input_depth_info_topic1_cmd, threshold1_cmd, yolo_encoding1_cmd,
            enable1_cmd, imgsz_height1_cmd, imgsz_width1_cmd, image_reliability1_cmd,
            yolo_node1, tracking_node1, debug_node1,
            # 相机2参数和节点
            #namespace2_cmd, input_image_topic2_cmd, input_depth_topic2_cmd,
            #input_depth_info_topic2_cmd, threshold2_cmd, yolo_encoding2_cmd,
            #enable2_cmd, imgsz_height2_cmd, imgsz_width2_cmd, image_reliability2_cmd,
            #yolo_node2, tracking_node2, debug_node2,
        ]

    # 顶层参数（跟踪和3D检测开关）
    use_tracking = LaunchConfiguration("use_tracking")
    use_tracking_cmd = DeclareLaunchArgument(
        "use_tracking", default_value="True", description="是否启用跟踪"
    )

    use_3d = LaunchConfiguration("use_3d")
    use_3d_cmd = DeclareLaunchArgument(
        "use_3d", default_value="False", description="是否启用3D检测"
    )

    return LaunchDescription([
        use_tracking_cmd,
        use_3d_cmd,
        OpaqueFunction(function=run_two_cameras, args=[use_tracking, use_3d]),
    ])

