import numpy as np
import math

class PurePursuitController:
    """
    纯跟踪控制器，适配车体坐标系下的局部参考轨迹。
    横向：几何预瞄计算转向角。
    纵向：比例控制，基于速度误差计算加速度/制动力。
    终点停车逻辑：基于轨迹剩余距离，线性减速至停车。
    """
    def __init__(self, params):
        # 车辆参数
        self.wheel_base = params['wheel_base']
        self.max_steering_angle = params['max_steering_angle']
        self.max_acceleration = params['max_acceleration']
        self.max_deceleration = params['max_deceleration']

        # 纯追踪参数
        self.lookahead_k = params['lookahead_k']
        self.lookahead_c = params['lookahead_c']

        # PID 参数
        self.kp = params['kp']
        self.ki = params['ki']
        self.kd = params['kd']
        self.dt = params['dt']
        self.prev_error = 0.0
        self.integral_error = 0.0
        self.prev_acceleration = 0.0
        self.prev_steering = 0.0

        # 目标速度
        self.original_target_speed = params['target_speed']
        self.target_speed = self.original_target_speed
        self.speed_change_threshold = params.get('speed_change_threshold', 10.0)
        self.new_target_speed = params.get('new_target_speed', 2.0)

        # 仿真相关
        self.mode = params.get('mode', 'simulation')
        if self.mode == 'simulation':
            self.vehicle_mass = params['vehicle_mass']
            self.wheel_radius = params['wheel_radius']
            self.air_density = params['air_density']
            self.frontal_area = params['frontal_area']
            self.drag_coefficient = params['drag_coefficient']
            self.rolling_resistance_coefficient = params['rolling_resistance_coefficient']
            self.gravity = params['gravity']
            self.drivetrain_efficiency = params['drivetrain_efficiency']
            self.static_friction_coefficient = params['static_friction_coefficient']
            self.min_driving_torque = params['min_driving_torque']

        # 轨迹存储
        self.trajectory = []
        self.remaining_distance = float('inf')

        # 状态
        self.is_finished = False
        self.current_speed = 0.0
        self.speed_changed = False

    def update_speed_from_car_state(self, car_state_msg):
        """从 CarState 消息更新车辆速度（车体坐标系合速度）。"""
        vx = car_state_msg.car_state_dt.car_state_dt.x
        vy = car_state_msg.car_state_dt.car_state_dt.y
        self.current_speed = math.hypot(vx, vy)

    def update_trajectory(self, trajectory_msg):
        """更新参考轨迹，并提取剩余距离信息。"""
        self.trajectory = []
        for pt in trajectory_msg.trajectory:
            self.trajectory.append({
                'x': pt.pts.x,
                'y': pt.pts.y,
                'yaw': pt.yaw.data,
                'velocity': pt.velocity.data,
            })
        self.remaining_distance = trajectory_msg.remaining_distance.data \
            if hasattr(trajectory_msg, 'remaining_distance') else float('inf')

    def find_lookahead_point(self, current_speed):
        """在车体坐标系轨迹中寻找预瞄点。"""
        if not self.trajectory:
            return None

        Ld = self.lookahead_k * max(0.1, abs(current_speed)) + self.lookahead_c

        accumulated_dist = 0.0
        for i in range(len(self.trajectory) - 1):
            x1 = self.trajectory[i]['x']
            y1 = self.trajectory[i]['y']
            x2 = self.trajectory[i+1]['x']
            y2 = self.trajectory[i+1]['y']

            seg_len = math.hypot(x2 - x1, y2 - y1)
            if seg_len < 1e-6:
                continue

            if accumulated_dist + seg_len >= Ld:
                remaining = Ld - accumulated_dist
                ratio = remaining / seg_len
                look_x = x1 + ratio * (x2 - x1)
                look_y = y1 + ratio * (y2 - y1)
                return (look_x, look_y)
            accumulated_dist += seg_len

        return (self.trajectory[-1]['x'], self.trajectory[-1]['y'])

    def compute_steering(self, lookahead_point):
        """纯跟踪转向角计算。"""
        if lookahead_point is None:
            return 0.0
        lx, ly = lookahead_point
        L2 = lx**2 + ly**2
        if L2 < 1e-6:
            return 0.0
        curvature = 2 * ly / L2
        steering = math.atan(curvature * self.wheel_base)
        steering = np.clip(steering, -self.max_steering_angle, self.max_steering_angle)
        # 低通滤波平滑转向
        steering = 0.7 * steering + 0.3 * self.prev_steering  # 低通平滑
        self.prev_steering = steering
        return steering

    def adjust_target_speed_based_on_end(self):
        """
        根据到轨迹终点的剩余距离调整目标速度。
        接近终点时逐步减速，距离<0.5m时强制刹停。
        """
        if self.remaining_distance >= self.speed_change_threshold:
            self.target_speed = self.original_target_speed
            return

        if self.remaining_distance < 0.5:
            self.target_speed = 0.0
            self.speed_changed = True
            return

        # v^2 = 2*a*d  →  v = sqrt(2 * |a| * remaining_distance)
        a = abs(self.max_deceleration)
        self.target_speed = max(0.0, math.sqrt(2.0 * a * self.remaining_distance))
        self.speed_changed = True

    def control_step(self):
        """
        执行一次控制计算，返回字典：
          steering_angle, acceleration, braking_acceleration, driving_torque.
        如果轨迹为空，返回零指令。
        """
        if not self.trajectory:
            return {
                'steering_angle': 0.0,
                'acceleration': 0.0,
                'driving_torque': 0.0,
                'braking_acceleration': 0.0,
                'current_speed': 0.0,
            }

        v = self.current_speed

        # ---- 终点减速逻辑 ----
        self.adjust_target_speed_based_on_end()

        # ---- 横向控制 ----
        lookahead = self.find_lookahead_point(v)
        steering = self.compute_steering(lookahead)

        # ---- 纵向控制：PID + 低通滤波 ----
        speed_error = self.target_speed - v

        # PID 计算 (死区: |误差|<0.3m/s清零积分防过冲)
        if abs(speed_error) < 0.3:
            acc_raw = 0.0
            self.integral_error = 0.0
        else:
            self.integral_error += speed_error * self.dt
            self.integral_error = np.clip(self.integral_error, -0.3, 0.3)
            derivative_error = (speed_error - self.prev_error) / self.dt if self.dt > 0 else 0.0
            self.prev_error = speed_error

            acc_raw = (self.kp * speed_error +
                       self.ki * self.integral_error +
                       self.kd * derivative_error)

        # 低通滤波平滑
        alpha = 0.85
        acc = alpha * acc_raw + (1.0 - alpha) * self.prev_acceleration
        self.prev_acceleration = acc

        # 加速度限幅
        acc = np.clip(acc, self.max_deceleration, self.max_acceleration)

        # 终点刹停: 距离<0.5m且目标速度=0时, 强制制动
        if self.remaining_distance < 0.5 and self.target_speed == 0.0:
            out_acc = 0.0
            out_brake = abs(self.max_deceleration)
        elif acc > 0.05:
            out_acc = acc
            out_brake = 0.0
        elif acc < -0.05:
            out_acc = 0.0
            out_brake = abs(acc)
        else:
            out_acc = 0.0
            out_brake = 0.0

        # 完成标志: 距离<0.5m且目标速度=0且车速<0.2m/s
        if self.remaining_distance < 0.5 and self.target_speed == 0.0 and abs(v) < 0.2:
            self.is_finished = True

        return {
            'steering_angle': steering,
            'acceleration': out_acc,
            'driving_torque': 0.0,
            'braking_acceleration': out_brake,
            'current_speed': v,
        }
