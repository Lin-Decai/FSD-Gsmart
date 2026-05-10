#!/usr/bin/env python3
"""
订阅 /hesai/cone_positions (PoseArray) 后存入文件，
用于与 PCD 参考地图比对验证聚类精度。

用法:
    source /opt/ros/humble/setup.bash
    source lidar/lidar_cone_detector/install/setup.bash
    python3 record_cones.py

输出: lidar/lidar_cone_detector/tools/recorded_cones.txt
      每行: x y z point_count
"""

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PoseArray
import os

OUTPUT_DIR = os.path.dirname(os.path.abspath(__file__))
OUTPUT_FILE = os.path.join(OUTPUT_DIR, 'recorded_cones.txt')


class ConeRecorder(Node):
    def __init__(self):
        super().__init__('cone_recorder')
        self.sub = self.create_subscription(
            PoseArray, '/hesai/cone_positions', self.callback, 10)
        self.recorded = False
        self.get_logger().info(
            f'Listening on /hesai/cone_positions, output → {OUTPUT_FILE}')

    def callback(self, msg: PoseArray):
        if self.recorded:
            return

        if not msg.poses:
            self.get_logger().warn('Received empty cone list, waiting...')
            return

        with open(OUTPUT_FILE, 'w') as f:
            f.write(f'# Recorded cone positions from /hesai/cone_positions\n')
            f.write(f'# frame_id: {msg.header.frame_id}\n')
            f.write(f'# count: {len(msg.poses)}\n')
            f.write(f'# x y z point_count\n')

            for i, pose in enumerate(msg.poses):
                x = pose.position.x 
                y = pose.position.y
                z = pose.position.z
                pts = int(pose.orientation.w)  # 聚类点数编码在 orientation.w
                f.write(f'{x:.6f} {y:.6f} {z:.6f} {pts}\n')

        self.get_logger().info(
            f'Recorded {len(msg.poses)} cones → {OUTPUT_FILE}')
        self.recorded = True


def main():
    rclpy.init()
    node = ConeRecorder()
    try:
        while rclpy.ok() and not node.recorded:
            rclpy.spin_once(node, timeout_sec=1.0)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
