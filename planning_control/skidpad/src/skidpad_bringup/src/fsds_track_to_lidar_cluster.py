#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
import sensor_msgs.msg as sensor_msgs
import std_msgs.msg as std_msgs
from fs_msgs.msg import Track
import sensor_msgs_py.point_cloud2 as pc2
import numpy as np

class TrackToLidarCluster(Node):
    def __init__(self):
        super().__init__('fsds_track_to_lidar_cluster')
        self.sub = self.create_subscription(Track, '/fsds/testing_only/track', self.callback, 10)
        self.pub = self.create_publisher(sensor_msgs.PointCloud2, '/perception/lidar_cluster', 10)
        self.get_logger().info('FSDS Track -> LiDAR Cluster bridge started')

    def callback(self, track_msg):
        points = []
        for cone in track_msg.track:
            # 每个锥桶的位置 (世界坐标系)
            x = cone.location.x
            y = cone.location.y
            z = 0.0
            points.append([x, y, z])

        if not points:
            return

        header = std_msgs.Header()
        header.stamp = self.get_clock().now().to_msg()
        header.frame_id = 'map'

        fields = [
            sensor_msgs.PointField(name='x', offset=0, datatype=sensor_msgs.PointField.FLOAT32, count=1),
            sensor_msgs.PointField(name='y', offset=4, datatype=sensor_msgs.PointField.FLOAT32, count=1),
            sensor_msgs.PointField(name='z', offset=8, datatype=sensor_msgs.PointField.FLOAT32, count=1),
        ]
        cloud = pc2.create_cloud(header, fields, points)
        self.pub.publish(cloud)

def main():
    rclpy.init()
    node = TrackToLidarCluster()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()