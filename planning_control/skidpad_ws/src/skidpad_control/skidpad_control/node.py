import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64, Bool
from skidpad_msgs.msg import Trajectory, ControlCommand, CarState
from .controller import PurePursuitController
import math


class PurePursuitNode(Node):
    def __init__(self):
        super().__init__('pure_pursuit_controller')

        # ---------- 声明参数 ----------
        self.declare_parameters(
            '',
            [
                ('wheel_base', 2.2),
                ('target_speed', 4.0),
                ('dt', 0.05),
                ('max_steering_angle', 0.436),
                ('max_acceleration', 0.8),
                ('max_deceleration', -3.0),
                ('kp', 5.0),
                ('ki', 1.5),
                ('kd', 1.0),
                ('lookahead_k', 0.5),
                ('lookahead_c', 1.0),
                ('speed_change_threshold', 10.0),
                ('new_target_speed', 2.0),
                ('vehicle_mass', 300.0),
                ('wheel_radius', 0.228),
                ('air_density', 1.225),
                ('frontal_area', 1.4),
                ('drag_coefficient', 0.7),
                ('rolling_resistance_coefficient', 0.015),
                ('gravity', 9.81),
                ('drivetrain_efficiency', 0.85),
                ('static_friction_coefficient', 0.0),
                ('min_driving_torque', 32.5),
                ('mode', 'simulation'),
                ('control_command_topic', '/control_command'),
                ('car_state_topic', '/estimation/slam/state'),
                ('ref_path_topic', '/planning/ref_path'),
                ('go_cmd_topic', '/go_cmd'),
            ]
        )

        mode = self.get_parameter('mode').value
        ref_path_topic = self.get_parameter('ref_path_topic').value
        car_state_topic = self.get_parameter('car_state_topic').value
        go_cmd_topic = self.get_parameter('go_cmd_topic').value
        cmd_topic = self.get_parameter('control_command_topic').value

        params = self._collect_controller_params()
        params['mode'] = mode
        self.controller = PurePursuitController(params)

        self.go_signal = False
        self.trajectory_received = False
        self._finished_printed = False
        self._stopped_printed = False

        # ---------- 订阅器 ----------
        self.traj_sub = self.create_subscription(
            Trajectory, ref_path_topic, self.traj_callback, 10)

        self.car_state_sub = self.create_subscription(
            CarState, car_state_topic, self.car_state_callback, 10)

        self.go_sub = self.create_subscription(
            Bool, go_cmd_topic, self.go_cmd_callback, 10)

        # ---------- 发布器 ----------
        self.cmd_pub = self.create_publisher(
            ControlCommand, cmd_topic, 10)

        self.finish_pub = self.create_publisher(Float64, '/as_finish', 10)

        # 控制定时器
        self.dt = self.get_parameter('dt').value
        self.timer = self.create_timer(self.dt, self.control_loop)

        self.get_logger().info(f'Pure Pursuit Controller ready in {mode} mode')

    def _collect_controller_params(self):
        keys = [
            'wheel_base', 'target_speed', 'dt', 'max_steering_angle',
            'max_acceleration', 'max_deceleration', 'lookahead_k', 'lookahead_c',
            'speed_change_threshold', 'new_target_speed',
            'vehicle_mass', 'wheel_radius', 'air_density', 'frontal_area',
            'drag_coefficient', 'rolling_resistance_coefficient', 'gravity',
            'drivetrain_efficiency', 'static_friction_coefficient',
            'min_driving_torque', 'kp', 'ki', 'kd',
        ]
        params = {}
        for key in keys:
            if self.has_parameter(key):
                params[key] = self.get_parameter(key).value
        return params

    def traj_callback(self, msg: Trajectory):
        self.controller.update_trajectory(msg)
        self.trajectory_received = True

    def car_state_callback(self, msg: CarState):
        vx = msg.car_state_dt.car_state_dt.x
        vy = msg.car_state_dt.car_state_dt.y
        self.controller.current_speed = math.hypot(vx, vy)

    def go_cmd_callback(self, msg: Bool):
        self.go_signal = msg.data
        if self.go_signal:
            self.get_logger().info('GO signal received')

    def control_loop(self):
        if not self.go_signal or not self.trajectory_received:
            return

        output = self.controller.control_step()

        cmd = ControlCommand()
        cmd.header.stamp = self.get_clock().now().to_msg()

        # 纵向控制映射 (高速需更大油门)
        if output['acceleration'] > 0.01:
            cmd.throttle.data = min(0.85, output['acceleration'] * 0.15)
        elif output['braking_acceleration'] > 0.01:
            cmd.throttle.data = -min(1.0, output['braking_acceleration'] / abs(self.controller.max_deceleration))
        else:
            cmd.throttle.data = 0.03

        # 横向控制映射 (纯跟踪正值=左转 → FSDS负值=左转)
        cmd.steering_angle.data = -output['steering_angle'] / self.controller.max_steering_angle

        self.cmd_pub.publish(cmd)

        # 完成标志 (只打印一次)
        if self.controller.is_finished:
            finish_msg = Float64()
            finish_msg.data = 1000.0
            self.finish_pub.publish(finish_msg)
            if not self._finished_printed:
                self.get_logger().info('Vehicle reached finish line.')
                self._finished_printed = True
            if self.controller.current_speed < 0.1 and not self._stopped_printed:
                self.get_logger().info('Car stopped successfully.')
                self._stopped_printed = True


def main(args=None):
    rclpy.init(args=args)
    node = PurePursuitNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
