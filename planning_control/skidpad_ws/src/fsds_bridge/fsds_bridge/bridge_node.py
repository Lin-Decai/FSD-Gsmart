#!/usr/bin/env python3
"""
FSDS Bridge: 连接 FSDS 仿真器与 ROS2 规控节点。
统一使用目标坐标系: X=北(直道), Y=西(左侧), 原点=车出生点。
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
import json

# FSDS Python 客户端
FSDS_PYTHON_PATH = os.path.expanduser('~/Formula-Student-Driverless-Simulator/python')
if FSDS_PYTHON_PATH not in sys.path:
    sys.path.insert(0, FSDS_PYTHON_PATH)
import fsds

class FSDSBridge(Node):
    def __init__(self):
        super().__init__('fsds_bridge')

        self.declare_parameter('cone_rate', 10)
        self.declare_parameter('state_rate', 100)
        self.declare_parameter('add_cone_noise', False)

        cone_rate = self.get_parameter('cone_rate').value
        state_rate = self.get_parameter('state_rate').value
        self.add_cone_noise = self.get_parameter('add_cone_noise').value

        # 连接 FSDS
        self.get_logger().info('正在连接 FSDS 仿真器...')
        try:
            self.client = fsds.FSDSClient()
            self.client.confirmConnection()
            self.client.enableApiControl(True)
            self.get_logger().info('FSDS 连接成功，API控制已启用')
        except Exception as e:
            self.get_logger().error(f'连接 FSDS 失败: {e}')
            raise

        # 读取车辆出生偏移
        self.veh_offset_x, self.veh_offset_y = self._read_vehicle_offset()

        # 锥桶缓存
        self.cones_cache = []
        self.cones_stamp = self.get_clock().now()
        self._cones_reported = False

        # 发布器
        self.state_pub = self.create_publisher(CarState, '/estimation/slam/state', 10)
        self.cone_pub = self.create_publisher(
            sensor_msgs.PointCloud2, '/perception/lidar_cluster', 10)
        self.go_pub = self.create_publisher(Bool, '/go_cmd', 10)

        # 订阅器
        self.cmd_sub = self.create_subscription(
            ControlCommand, '/control_command', self.control_callback, 10)

        # 定时器
        self.state_timer = self.create_timer(1.0 / state_rate, self.state_callback)
        self.cone_timer = self.create_timer(1.0 / cone_rate, self.cone_callback)

        # 启动时立即获取锥桶
        self.update_cones()
        self._publish_go_signal()

        self.get_logger().info(f'FSDS Bridge 就绪 (state={state_rate}Hz, cone={cone_rate}Hz)')

    def _read_vehicle_offset(self):
        """读取 settings.json 中的车辆偏移 (m)。"""
        try:
            path = os.path.expanduser('~/Formula-Student-Driverless-Simulator/settings.json')
            with open(path) as f:
                s = json.load(f)
            v = s['Vehicles']['FSCar']
            return v.get('X', 0.0), v.get('Y', 0.0)
        except Exception:
            return 0.0, 0.0

    def _publish_go_signal(self):
        msg = Bool()
        msg.data = True
        self.go_pub.publish(msg)
        self.get_logger().info('仿真 GO 信号已发送')

    # ============================================================
    #  锥桶获取与发布 (目标坐标系: X=北=直道, Y=西=左侧)
    # ============================================================
    def update_cones(self):
        try:
            referee = self.client.getRefereeState()
            ip = referee.initial_position
            if isinstance(ip, dict):
                ps_x_cm, ps_y_cm = ip.get('x', 0.0), ip.get('y', 0.0)
            else:
                ps_x_cm, ps_y_cm = ip.x, ip.y
            # 车出生点 = PlayerStart + settings偏移
            spawn_x_cm = ps_x_cm + self.veh_offset_x * 100.0
            spawn_y_cm = ps_y_cm + self.veh_offset_y * 100.0

            cones = []
            for cone in referee.cones:
                if isinstance(cone, dict):
                    ux, uy = cone.get('x', 0.0), cone.get('y', 0.0)
                    c = cone.get('color', -1)
                else:
                    ux, uy = cone.x, cone.y
                    c = cone.color
                # UE4 cm (X=北,Y=东) → 目标坐标系 (X=北,Y=西, 原点=车出生点)
                enu_x = (uy - spawn_y_cm) / 100.0   # 东(右侧)
                enu_y = (ux - spawn_x_cm) / 100.0   # 北(直道)
                tx = enu_y   # 北→X (直道)
                ty = -enu_x  # 东→取反→西 (左侧)
                cones.append({'x': tx, 'y': ty, 'z': 0.0, 'color': c})
            self.cones_cache = cones
            self.cones_stamp = self.get_clock().now()
            if not self._cones_reported:
                self.get_logger().info(f'获取到 {len(cones)} 个锥桶')
                self._cones_reported = True
        except Exception as e:
            self.get_logger().warn(f'获取锥桶失败: {e}', throttle_duration_sec=5)

    def cone_callback(self):
        self.update_cones()
        if not self.cones_cache:
            return
        points = []
        for cone in self.cones_cache:
            x, y = cone['x'], cone['y']
            if self.add_cone_noise:
                import random
                x += random.gauss(0, 0.05)
                y += random.gauss(0, 0.05)
            points.append([x, y, 0.0])
        header = std_msgs.Header()
        header.stamp = self.cones_stamp.to_msg()
        header.frame_id = 'map'
        fields = [
            sensor_msgs.PointField(name='x', offset=0, datatype=sensor_msgs.PointField.FLOAT32, count=1),
            sensor_msgs.PointField(name='y', offset=4, datatype=sensor_msgs.PointField.FLOAT32, count=1),
            sensor_msgs.PointField(name='z', offset=8, datatype=sensor_msgs.PointField.FLOAT32, count=1),
        ]
        self.cone_pub.publish(pc2.create_cloud(header, fields, points))

    # ============================================================
    #  车辆状态获取与发布 (目标坐标系: X=北=直道, Y=西=左侧)
    # ============================================================

    def state_callback(self):
        try:
            fsds_state = self.client.getCarState()
        except Exception as e:
            self.get_logger().warn(f'获取车辆状态失败: {e}', throttle_duration_sec=5)
            return

        kin = fsds_state.kinematics_estimated

        # getCarState() 返回: X=北(直道), Y=东(右侧), yaw:0=北
        # 车位: 目标坐标系 (X=北=直道, Y=西=左侧), 与轨迹/PCD/锥桶一致
        enu_x = kin.position.x_val   # 北(直道)
        enu_y = kin.position.y_val   # 东(右侧)
        px = enu_x                   # 北 → X (不变)
        py = -enu_y                  # 东→取反→西(左侧) — 目标坐标系

        # 速度 (目标坐标系: 东速→取反→西速)
        vx = kin.linear_velocity.x_val
        vy = -kin.linear_velocity.y_val
        self._last_vx = vx
        self._last_vy = vy
        wz = kin.angular_velocity.z_val
        ax = kin.linear_acceleration.x_val
        ay = -kin.linear_acceleration.y_val

        # 航向 (yaw:0=北, 目标坐标系与getCarState一致, 无需转换)
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

    # ============================================================
    #  控制指令下发
    # ============================================================
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
            self.get_logger().warn(f'发送控制指令失败: {e}', throttle_duration_sec=5)

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
