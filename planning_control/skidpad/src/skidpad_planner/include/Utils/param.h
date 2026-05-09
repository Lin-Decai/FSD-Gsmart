#pragma once

#include "rclcpp/rclcpp.hpp"

/**
 * 全局参数结构体，从 ROS2 参数服务器读取配置
 */
struct Param {
    int N;                     // 规划路径点数量
    double dt;                 // 规划步长时间
    bool simulation;           // 是否在仿真模式下运行（影响车辆参数处理）

    double car_length;         // 车辆轴距（或换算到质心距离）
    double initial_velocity;   // 初始速度

    double interval;           // 轨迹点间距（米）
    double forward_distance;   // 直线段额外长度（理想坐标系）
    double circle_radius;      // 圆形轨迹半径
    double max_lat_acc;        // 最大侧向加速度极限（用于计算转弯速度）

    double desire_vel;         // 期望的直线速度

    void getParams(rclcpp::Node::SharedPtr node) {
        car_length       = node->declare_parameter<double>("car_length", 1.88);
        N                = node->declare_parameter<int>("N", 40);
        dt               = node->declare_parameter<double>("dt", 0.04);
        simulation       = node->declare_parameter<bool>("simulation", true);
        interval         = node->declare_parameter<double>("interval", 0.08);
        forward_distance = node->declare_parameter<double>("forward_distance", 15.0);
        circle_radius    = node->declare_parameter<double>("circle_radius", 9.125);
        max_lat_acc      = node->declare_parameter<double>("max_lat_acc", 3.0);
        initial_velocity = node->declare_parameter<double>("initial_velocity", 2.0);
        desire_vel       = node->declare_parameter<double>("desire_vel", 3.0);
    }
};

extern Param param_;