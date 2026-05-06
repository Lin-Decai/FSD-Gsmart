#ifndef SKIDPAD_DETECTOR_HPP
#define SKIDPAD_DETECTOR_HPP

// ROS2 核心头文件
#include <rclcpp/rclcpp.hpp>

// ROS2 消息类型
#include <sensor_msgs/msg/point_cloud2.hpp>          // 激光雷达点云消息
#include <std_msgs/msg/float64_multi_array.hpp>      // 用于发布变换矩阵
#include <geometry_msgs/msg/pose_stamped.hpp>        // 路径点（未使用但保留）
#include <geometry_msgs/msg/point32.hpp>             // 用于存储地图点云中的点
#include <nav_msgs/msg/path.hpp>                     // 路径消息（未使用但保留）

// PCL (点云库) 相关头文件
#include <pcl/io/pcd_io.h>                           // 加载 PCD 文件
#include <pcl/registration/icp.h>                    // ICP 配准算法
#include <pcl_conversions/pcl_conversions.h>         // ROS2 点云与 PCL 点云互转

// 标准库
#include <fstream>   // 文件流，读取 txt 路径文件
#include <cmath>     // 数学函数，如 hypot
#include <Eigen/Dense> // 矩阵运算，存储变换矩阵

namespace ns_skidpad_detector {

/**
 * @brief 核心算法类：负责 ICP 匹配，计算激光雷达与标准地图的变换矩阵
 * 
 * 该类不直接处理 ROS 通信，而是由 SkidpadDetectorHandle 调用。
 * 主要流程：
 *   1. 加载标准地图（PCD 点云）和标准路径（txt 文件）
 *   2. 收到实际点云数据后，进行最近邻匹配，找到对应点对
 *   3. 使用 ICP 计算变换矩阵
 */
class SkidpadDetector {
public:
  /**
   * @brief 构造函数
   * @param nh ROS2 节点的共享指针，用于访问参数和日志
   */
  SkidpadDetector(rclcpp::Node::SharedPtr nh);

  /**
   * @brief 获取计算出的变换矩阵，并包装成 ROS2 消息
   * @return std_msgs::msg::Float64MultiArray 4x4 矩阵展开为一维数组
   */
  std_msgs::msg::Float64MultiArray getTransMatrix();
  bool getMatchFlag() { return matchFlag; }

  /**
   * @brief 设置接收到的点云数据（来自激光雷达）
   * @param msg ROS2 点云消息的共享指针
   */
  void setclusterFiltered(const sensor_msgs::msg::PointCloud2::SharedPtr msg);

  /**
   * @brief 运行 ICP 匹配算法（由定时器周期调用）
   */
  void runAlgorithm();

private:
  // ROS2 节点指针，用于获取参数和打印日志
  rclcpp::Node::SharedPtr nh_;

  // ---------- 参数 ----------
  std::string path_pcd_;     // 标准地图 PCD 文件路径
  double start_length_;      // 起点到赛道中心的距离（单位：米）
  double lidar2imu_;         // 激光雷达到 IMU 的距离（用于坐标补偿）
  double threshold_;         // 最近邻匹配的距离阈值

  double icp_max_correspondence_distance_;
  double icp_transformation_epsilon_;
  double icp_euclidean_fitness_epsilon_;
  int icp_max_iterations_;

  // ---------- 状态标志 ----------
  bool getClusterFlag;   // 是否已经收到激光雷达点云
  bool matchFlag;        // 是否已经完成一次匹配（只匹配一次）

  // ---------- 数据 ----------
  sensor_msgs::msg::PointCloud2 cluster_;   // 存储接收到的点云
  std::vector<geometry_msgs::msg::Point32> skidpad_map_points_; // 存储地图点云（转换坐标后）

  // ---------- 输出 ----------
  std_msgs::msg::Float64MultiArray trans_matrix_in_1D_; // 发布用的消息
  Eigen::Matrix4f transformation_;                      // 存储 4x4 变换矩阵

  // ---------- 内部方法 ----------
  void loadParameters();   // 从 ROS2 参数服务器加载参数
  void loadFiles();        // 加载地图文件和路径文件
  /**
   * @brief 将 ROS2 点云消息转换为 PCL 点云指针
   * @param msg ROS2 点云消息
   * @return PCL 点云智能指针
   */
  pcl::PointCloud<pcl::PointXYZ>::Ptr ros2ToPcl(const sensor_msgs::msg::PointCloud2::SharedPtr &msg);
};

} // namespace ns_skidpad_detector

#endif // SKIDPAD_DETECTOR_HPP