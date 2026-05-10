#include "skidpad_detector.hpp"

#include <cmath>
#include <limits>

namespace ns_skidpad_detector {

// 构造函数：初始化标志位，加载参数和文件
SkidpadDetector::SkidpadDetector(rclcpp::Node::SharedPtr nh) : nh_(nh) {
  getClusterFlag = false;  // 初始未收到点云
  matchFlag = false;       // 初始未完成匹配
  loadParameters();        // 从 YAML 文件加载参数
  loadFiles();             // 加载标准地图和路径文件
  transformation_ = Eigen::Matrix4f::Identity();
}

// 获取变换矩阵，转换为 ROS2 消息格式
std_msgs::msg::Float64MultiArray SkidpadDetector::getTransMatrix() {
  trans_matrix_in_1D_.data.clear();  // 清空数组
  // 将 4x4 矩阵按行优先顺序展平为一维数组
  for (int i = 0; i < transformation_.rows(); i++)
    for (int j = 0; j < transformation_.cols(); j++) {
      trans_matrix_in_1D_.data.push_back(transformation_(i, j));
    }
  return trans_matrix_in_1D_;
}

// 设置接收到的点云数据
void SkidpadDetector::setclusterFiltered(const sensor_msgs::msg::PointCloud2::SharedPtr msg) {
  getClusterFlag = true;  // 标记已收到点云
  cluster_ = *msg;        // 拷贝消息内容（深拷贝）
}

// 从 ROS2 参数服务器加载参数
void SkidpadDetector::loadParameters() {
  RCLCPP_INFO(nh_->get_logger(), "loading parameters");

  // 声明参数并设置默认值
  nh_->declare_parameter<std::string>("path.skidpad_map", "skidpad_detector/skidpad.pcd");
  nh_->declare_parameter<double>("length.start", 15.0);
  nh_->declare_parameter<double>("length.lidar2imu", 1.87);
  nh_->declare_parameter<double>("length.threshold", 0.7);

  nh_->declare_parameter<double>("icp.max_correspondence_distance", 1.0);
  nh_->declare_parameter<double>("icp.transformation_epsilon", 1e-10);
  nh_->declare_parameter<double>("icp.euclidean_fitness_epsilon", 0.001);
  nh_->declare_parameter<int>("icp.max_iterations", 100);

  // 获取实际参数值
  nh_->get_parameter("path.skidpad_map", path_pcd_);
  nh_->get_parameter("length.start", start_length_);
  nh_->get_parameter("length.lidar2imu", lidar2imu_);
  nh_->get_parameter("length.threshold", threshold_);
  
  nh_->get_parameter("icp.max_correspondence_distance", icp_max_correspondence_distance_);
  nh_->get_parameter("icp.transformation_epsilon", icp_transformation_epsilon_);
  nh_->get_parameter("icp.euclidean_fitness_epsilon", icp_euclidean_fitness_epsilon_);
  nh_->get_parameter("icp.max_iterations", icp_max_iterations_);
}

// 加载标准地图文件（PCD）和标准路径文件（txt）
void SkidpadDetector::loadFiles() {
  // ---------- 加载 PCD 地图 ----------
  pcl::PointCloud<pcl::PointXYZ> source_cloud;
  if (pcl::io::loadPCDFile<pcl::PointXYZ>(path_pcd_, source_cloud) == -1) {
    RCLCPP_ERROR(nh_->get_logger(), "Couldn't read file %s", path_pcd_.c_str());
    return;
  }
  RCLCPP_INFO(nh_->get_logger(), "Loaded PCD file with %zu points", source_cloud.points.size());

    // PCD 文件已直接存储目标坐标系 (X=直道=北, Y=左侧=西), 加载时加上雷达前向偏移转为传感器系
  for (const auto& point : source_cloud.points) {
    geometry_msgs::msg::Point32 tmp;
    tmp.x = point.x + static_cast<float>(lidar2imu_);
    tmp.y = point.y;
    tmp.z = 0.0;
    skidpad_map_points_.push_back(tmp);
  }}

// ROS2 点云 -> PCL 点云 转换
pcl::PointCloud<pcl::PointXYZ>::Ptr SkidpadDetector::ros2ToPcl(const sensor_msgs::msg::PointCloud2::SharedPtr &msg) {
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
  pcl::fromROSMsg(*msg, *cloud);  // 使用 PCL 提供的转换函数
  return cloud;
}

// 核心算法：ICP 匹配
void SkidpadDetector::runAlgorithm() {
  if (!getClusterFlag)
    return;

  auto cloud_in_pcl = ros2ToPcl(
      std::make_shared<sensor_msgs::msg::PointCloud2>(cluster_));

  // 锥桶坐标从传感器系转到与地图一致的坐标系: X += lidar2imu
  for (auto& pt : cloud_in_pcl->points) {
    pt.x += static_cast<float>(lidar2imu_);
  }

  pcl::PointCloud<pcl::PointXYZ>::Ptr source_cloud(new pcl::PointCloud<pcl::PointXYZ>);
  pcl::PointCloud<pcl::PointXYZ>::Ptr target_cloud(new pcl::PointCloud<pcl::PointXYZ>);

  // 最近邻匹配
  for (const auto& map_pt : skidpad_map_points_) {
    double min_dist = std::numeric_limits<double>::infinity();
    int index = -1;
    for (size_t j = 0; j < cloud_in_pcl->points.size(); ++j) {
      double dist = std::hypot(map_pt.x - cloud_in_pcl->points[j].x,
                               map_pt.y - cloud_in_pcl->points[j].y);
      if (dist < min_dist) {
        min_dist = dist;
        index = j;
      }
    }
    if (min_dist < threshold_) {
      pcl::PointXYZ in_pt;
      in_pt.x = map_pt.x;
      in_pt.y = map_pt.y;
      in_pt.z = 0;
      source_cloud->push_back(in_pt);

      pcl::PointXYZ out_pt;
      out_pt.x = cloud_in_pcl->points[index].x;
      out_pt.y = cloud_in_pcl->points[index].y;
      out_pt.z = 0;
      target_cloud->push_back(out_pt);
    }
  }

  // 检查匹配点对数量（至少需要3对才能计算刚体变换）
  if (source_cloud->points.size() < 3) {
    RCLCPP_WARN_ONCE(nh_->get_logger(),
                "Not enough correspondences: %zu, need at least 3. Skipping ICP.",
                source_cloud->points.size());
    return;
  }

  // ICP 配准
  pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp;
  icp.setInputSource(source_cloud);
  icp.setInputTarget(target_cloud);
  icp.setMaxCorrespondenceDistance(icp_max_correspondence_distance_);
  icp.setTransformationEpsilon(icp_transformation_epsilon_);
  icp.setEuclideanFitnessEpsilon(icp_euclidean_fitness_epsilon_);
  icp.setMaximumIterations(icp_max_iterations_);

  pcl::PointCloud<pcl::PointXYZ> final;
  try {
    RCLCPP_INFO_ONCE(nh_->get_logger(), "ICP params: max_corr_dist=%.2f, max_iter=%d, src=%zu, tgt=%zu",
            icp_max_correspondence_distance_, icp_max_iterations_,
            source_cloud->points.size(), target_cloud->points.size());
    icp.align(final);
  } catch (const std::exception& e) {
    RCLCPP_ERROR(nh_->get_logger(), "ICP failed with exception: %s", e.what());
    return;
  }

  if (!icp.hasConverged()) {
    RCLCPP_WARN_ONCE(nh_->get_logger(), "ICP did not converge, fitness score: %f",
                icp.getFitnessScore());
    return;
  }

  transformation_ = icp.getFinalTransformation();
  matchFlag = true;
  RCLCPP_INFO_ONCE(nh_->get_logger(), "ICP succeeded. Score: %f", icp.getFitnessScore());
}

} // namespace ns_skidpad_detector