#!/usr/bin/env python3
"""
轻量实时可视化：绘制全局轨迹、锥桶、参考路径、车辆位置和控制量。
"""

import rclpy
from rclpy.node import Node
from skidpad_msgs.msg import CarState, Trajectory, ControlCommand
from sensor_msgs.msg import PointCloud2
from visualization_msgs.msg import MarkerArray
import sensor_msgs_py.point_cloud2 as pc2
import numpy as np
import math


class Visualizer(Node):
    def __init__(self):
        super().__init__('skidpad_visualizer')

        self.car_x = 0.0
        self.car_y = 0.0
        self.car_yaw = 0.0
        self.car_speed = 0.0
        self.trajectory_points = []
        self.cones = []
        self.throttle = 0.0
        self.steering = 0.0
        self.global_path = []

        self.car_sub = self.create_subscription(
            CarState, '/estimation/slam/state', self.car_callback, 10)
        self.traj_sub = self.create_subscription(
            Trajectory, '/planning/ref_path', self.traj_callback, 10)
        self.cone_sub = self.create_subscription(
            PointCloud2, '/perception/lidar_cluster', self.cone_callback, 10)
        self.cmd_sub = self.create_subscription(
            ControlCommand, '/control_command', self.cmd_callback, 10)
        self.global_sub = self.create_subscription(
            MarkerArray, '/visual/global_path', self.global_callback, 10)

        # 用定时器更新绘图 (5Hz, 避免卡顿)
        self._setup_plot()
        self.plot_timer = self.create_timer(0.2, self._update_plot)

    def _setup_plot(self):
        import matplotlib
        matplotlib.use('TkAgg')
        import matplotlib.pyplot as plt

        self.fig, (self.ax1, self.ax2) = plt.subplots(1, 2, figsize=(14, 6))
        self.fig.show()
        self.fig.canvas.draw()
        plt.pause(0.01)

    def car_callback(self, msg: CarState):
        self.car_x = msg.car_state.x
        self.car_y = msg.car_state.y
        self.car_yaw = msg.car_state.theta
        vx = msg.car_state_dt.car_state_dt.x
        vy = msg.car_state_dt.car_state_dt.y
        self.car_speed = math.hypot(vx, vy)

    def traj_callback(self, msg: Trajectory):
        self.trajectory_points = [(pt.pts.x, pt.pts.y) for pt in msg.trajectory]

    def cone_callback(self, msg: PointCloud2):
        pts = []
        for pt in pc2.read_points(msg, field_names=('x', 'y'), skip_nans=True):
            pts.append((pt[0], pt[1]))
        self.cones = pts

    def cmd_callback(self, msg: ControlCommand):
        self.throttle = msg.throttle.data
        self.steering = msg.steering_angle.data

    def global_callback(self, msg: MarkerArray):
        pts = []
        for marker in msg.markers:
            for p in marker.points:
                pts.append((p.x, p.y))
        # 降采样到最多500点
        if len(pts) > 500:
            step = len(pts) // 500
            pts = pts[::step]
        self.global_path = pts

    def _update_plot(self):
        import matplotlib.pyplot as plt

        self.ax1.cla()
        self.ax2.cla()

        # --- 左图 ---
        self.ax1.set_title(f'Position & Path (speed: {self.car_speed:.1f} m/s)')
        self.ax1.set_xlabel('X (m)')
        self.ax1.set_ylabel('Y (m)')
        self.ax1.set_aspect('equal')
        self.ax1.grid(True, alpha=0.3)

        # 全局轨迹 (浅灰, 底层)
        if self.global_path:
            gx, gy = zip(*self.global_path)
            self.ax1.plot(gx, gy, color='lightgray', linewidth=3, zorder=1)

        # 锥桶 (橙色方块)
        if self.cones:
            cx, cy = zip(*self.cones)
            self.ax1.scatter(cx, cy, c='orange', s=6, marker='s', zorder=2, alpha=0.7)

        # 参考轨迹 (红色, 车体系→世界系)
        if self.trajectory_points:
            tx = [self.car_x + p[0]*math.cos(self.car_yaw) - p[1]*math.sin(self.car_yaw)
                  for p in self.trajectory_points]
            ty = [self.car_y + p[0]*math.sin(self.car_yaw) + p[1]*math.cos(self.car_yaw)
                  for p in self.trajectory_points]
            self.ax1.plot(tx, ty, 'r-', linewidth=1.5, zorder=3)

        # 车辆
        self.ax1.arrow(self.car_x, self.car_y,
                      1.5*math.cos(self.car_yaw), 1.5*math.sin(self.car_yaw),
                      head_width=0.6, head_length=0.9, fc='blue', ec='blue', zorder=4)
        self.ax1.scatter([self.car_x], [self.car_y], c='blue', s=30, zorder=4)

        # 视野: X(直道方向)多看点, Y(横向)对称
        self.ax1.set_xlim(self.car_x - 5, self.car_x + 30)
        self.ax1.set_ylim(self.car_y - 15, self.car_y + 15)

        # --- 右图: 控制量 ---
        self.ax2.set_title('Controls')
        self.ax2.set_ylim(-1.2, 1.2)
        self.ax2.bar(['throttle', 'steering'], [self.throttle, self.steering],
                     color=['green', 'red'], width=0.4)
        self.ax2.axhline(y=0, color='black', linewidth=0.5)
        for i, val in enumerate([self.throttle, self.steering]):
            self.ax2.text(i, val + 0.05*np.sign(val) if val != 0 else 0.05,
                         f'{val:.2f}', ha='center')

        self.fig.canvas.draw_idle()
        self.fig.canvas.flush_events()


def main(args=None):
    rclpy.init(args=args)
    node = Visualizer()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
