#!/usr/bin/env python3
"""
FSDS ROS Bridge: 连接 FSDS 仿真器与 ROS 2。
发布: 原始雷达点云 (/lidar_points)、车辆状态 (/estimation/slam/state)、GO 信号 (/go_cmd)
订阅: 控制指令 (/control_command)
"""

import rclpy
from rclpy.node import Node
import sensor_msgs.msg as sensor_msgs
import std_msgs.msg as std_msgs
from std_msgs.msg import Bool
from skidpad_msgs.msg import CarState, CarStateDt, ControlCommand
import sensor_msgs_py.point_cloud2 as pc2

import sys
import os
import math
import numpy as np

FSDS_PYTHON_PATH = os.path.expanduser('~/Formula-Student-Driverless-Simulator/python')
if FSDS_PYTHON_PATH not in sys.path:
    sys.path.insert(0, FSDS_PYTHON_PATH)
import fsds


def quat_to_rot_matrix(q):
    """四元数 [w, x, y, z] -> 3x3 旋转矩阵"""
    w, x, y, z = q[0], q[1], q[2], q[3]
    return np.array([
        [1 - 2*(y*y + z*z),     2*(x*y - w*z),     2*(x*z + w*y)],
        [    2*(x*y + w*z), 1 - 2*(x*x + z*z),     2*(y*z - w*x)],
        [    2*(x*z - w*y),     2*(y*z + w*x), 1 - 2*(x*x + y*y)]
    ], dtype=np.float32)


class FSDSBridge(Node):
    def __init__(self):
        super().__init__('fsds_bridge')

        self.declare_parameter('lidar_rate', 10)
        self.declare_parameter('state_rate', 100)
        self.declare_parameter('lidar_name', 'Lidar1')

        lidar_rate = self.get_parameter('lidar_rate').value
        state_rate = self.get_parameter('state_rate').value
        self.lidar_name = self.get_parameter('lidar_name').value

        # 连接 FSDS
        self.get_logger().info('Connecting to FSDS simulator...')
        try:
            self.client = fsds.FSDSClient()
            self.client.confirmConnection()
            self.client.enableApiControl(True)
            self.get_logger().info('FSDS connected, API control enabled')
        except Exception as e:
            self.get_logger().error(f'FSDS connection failed: {e}')
            raise

        # 发布器
        self.state_pub = self.create_publisher(CarState, '/estimation/slam/state', 10)
        self.lidar_pub = self.create_publisher(
            sensor_msgs.PointCloud2, '/lidar_points', 10)
        self.go_pub = self.create_publisher(Bool, '/go_cmd', 10)

        # 订阅器
        self.cmd_sub = self.create_subscription(
            ControlCommand, '/control_command', self.control_callback, 10)

        # 定时器
        self.state_timer = self.create_timer(1.0 / state_rate, self.state_callback)
        self.lidar_timer = self.create_timer(1.0 / lidar_rate, self.lidar_callback)

        # 启动时发 GO 信号
        self._publish_go_signal()

        self.get_logger().info(
            f'FSDS Bridge ready (state={state_rate}Hz, lidar={lidar_rate}Hz, {self.lidar_name})')

    def _publish_go_signal(self):
        msg = Bool()
        msg.data = True
        self.go_pub.publish(msg)
        self.get_logger().info('GO signal sent')

    # ================================================================
    #  原始雷达点云发布 (/lidar_points)
    #  世界坐标系 -> 传感器坐标系 (hesai_lidar: X=前, Y=左, Z=上)
    # ================================================================
    def lidar_callback(self):
        try:
            data = self.client.getLidarData(
                lidar_name=self.lidar_name, vehicle_name='FSCar')
        except Exception as e:
            self.get_logger().warn(
                f'Failed to get lidar data: {e}', throttle_duration_sec=5)
            return

        if not data.point_cloud:
            return

        # 原始点云 (世界坐标系, 交错 XYZ float 列表, 单位: 米)
        pts_world = np.array(data.point_cloud, dtype=np.float32).reshape(-1, 3)

        # 雷达在世界坐标系下的位姿
        lp = data.pose.position
        lo = data.pose.orientation
        lidar_pos = np.array([lp.x_val, lp.y_val, lp.z_val], dtype=np.float32)
        q = np.array([lo.w_val, lo.x_val, lo.y_val, lo.z_val], dtype=np.float32)

        # Step 1: 世界 -> AirSim 传感器坐标系 (X=前, Y=右, Z=上)
        pts_rel = pts_world - lidar_pos
        R = quat_to_rot_matrix(q)
        # R 是世界->传感器旋转, R^T = 传感器->世界.
        # p_sensor = R^T @ p_rel (世界点转到传感器系)
        pts_airsim = pts_rel @ R  # (N,3) @ (3,3) = (N,3)

        # Step 2: AirSim 传感器系 -> ROS hesai_lidar 传感器系 (Y 轴取反: 右->左)
        pts_ros = pts_airsim.copy()
        pts_ros[:, 1] = -pts_airsim[:, 1]

        # 发布 PointCloud2
        header = std_msgs.Header()
        header.stamp = self.get_clock().now().to_msg()
        header.frame_id = 'hesai_lidar'

        fields = [
            sensor_msgs.PointField(
                name='x', offset=0,
                datatype=sensor_msgs.PointField.FLOAT32, count=1),
            sensor_msgs.PointField(
                name='y', offset=4,
                datatype=sensor_msgs.PointField.FLOAT32, count=1),
            sensor_msgs.PointField(
                name='z', offset=8,
                datatype=sensor_msgs.PointField.FLOAT32, count=1),
        ]
        self.lidar_pub.publish(
            pc2.create_cloud(header, fields, pts_ros.tolist()))

    # ================================================================
    #  车辆状态发布 (目标坐标系: X=北=直道, Y=西=左侧)
    # ================================================================
    def state_callback(self):
        try:
            fsds_state = self.client.getCarState()
        except Exception as e:
            self.get_logger().warn(
                f'Failed to get car state: {e}', throttle_duration_sec=5)
            return

        kin = fsds_state.kinematics_estimated

        # getCarState(): X=北, Y=东, yaw:0=北
        # 目标坐标系: X=北, Y=西(左侧)
        enu_x = kin.position.x_val
        enu_y = kin.position.y_val
        px = enu_x
        py = -enu_y

        vx = kin.linear_velocity.x_val
        vy = -kin.linear_velocity.y_val
        wz = kin.angular_velocity.z_val
        ax = kin.linear_acceleration.x_val
        ay = -kin.linear_acceleration.y_val

        qw = kin.orientation.w_val
        qx = kin.orientation.x_val
        qy = kin.orientation.y_val
        qz = kin.orientation.z_val
        siny_cosp = 2.0 * (qw * qz + qx * qy)
        cosy_cosp = 1.0 - 2.0 * (qy * qy + qz * qz)
        yaw = math.atan2(siny_cosp, cosy_cosp)

        msg = CarState()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = 'map'
        msg.car_state.x = px
        msg.car_state.y = py
        msg.car_state.theta = yaw

        dt_msg = CarStateDt()
        dt_msg.header.stamp = msg.header.stamp
        dt_msg.car_state_dt.x = vx
        dt_msg.car_state_dt.y = vy
        dt_msg.car_state_dt.theta = wz
        dt_msg.car_state_a.x = ax
        dt_msg.car_state_a.y = ay
        dt_msg.car_state_a.theta = 0.0
        msg.car_state_dt = dt_msg

        self.state_pub.publish(msg)

    # ================================================================
    #  控制指令转发到 FSDS
    # ================================================================
    def control_callback(self, msg: ControlCommand):
        controls = fsds.CarControls()
        throttle_val = msg.throttle.data
        steering_val = msg.steering_angle.data

        if throttle_val >= 0:
            controls.throttle = min(1.0, throttle_val)
            controls.brake = 0.0
        else:
            controls.throttle = 0.0
            controls.brake = min(1.0, -throttle_val)
        controls.steering = steering_val

        try:
            self.client.setCarControls(controls)
        except Exception as e:
            self.get_logger().warn(
                f'Failed to send controls: {e}', throttle_duration_sec=5)


def main(args=None):
    rclpy.init(args=args)
    node = FSDSBridge()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
