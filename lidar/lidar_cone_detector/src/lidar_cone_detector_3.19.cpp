//ROI+RANSAC+欧几里德+形态学    
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "pcl_conversions/pcl_conversions.h"
#include "pcl/point_types.h"
#include "pcl/filters/passthrough.h"
#include "pcl/common/common.h"
#include "pcl/filters/extract_indices.h"
#include "pcl/segmentation/sac_segmentation.h"
#include "pcl/segmentation/extract_clusters.h"
#include "pcl/common/centroid.h"
#include "geometry_msgs/msg/pose_array.hpp"
#include <utility> 
#include <pcl/common/geometry.h>

class HesaiProcessor : public rclcpp::Node
{
public:
    HesaiProcessor() : Node("hesai_processor")
    {
        sub_lidar = this->create_subscription<sensor_msgs::msg::PointCloud2>(
            "/lidar_points",
            rclcpp::SensorDataQoS().keep_last(5),
            [this](const sensor_msgs::msg::PointCloud2::ConstSharedPtr msg) {
                this->pointCloudCallback(msg);
            });

        roi_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/hesai/roi_cloud", 10);
        ground_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/hesai/ground_filtered", 10);
        cluster_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/hesai/clustered", 10);
        cone_pub_ = this->create_publisher<geometry_msgs::msg::PoseArray>("/hesai/cone_positions", 10);
    }

private:
void pointCloudCallback(const sensor_msgs::msg::PointCloud2::ConstSharedPtr input)
{
    // 检查输入点云是否为空
    if (!input || input->data.empty()) {
        RCLCPP_WARN(this->get_logger(), "Received empty point cloud!");
        return;
    }

    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZI>());
    try {
        pcl::fromROSMsg(*input, *cloud);
    } catch (const std::exception& e) {
        RCLCPP_ERROR(this->get_logger(), "Failed to convert ROS message to PCL point cloud: %s", e.what());
        return;
    }

    // 移除无效点
    cloud->erase(std::remove_if(cloud->begin(), cloud->end(),
        [](const pcl::PointXYZI& point) {
            return !std::isfinite(point.x) || !std::isfinite(point.y) || !std::isfinite(point.z);
        }), cloud->end());

    RCLCPP_INFO(this->get_logger(), "Processing point cloud with %zu points", cloud->size());

    auto roi_cloud = extractROI(cloud);
    publishPointCloud(roi_cloud, roi_pub_);

    auto filtered_cloud = groundFilter(roi_cloud);
    publishPointCloud(filtered_cloud, ground_pub_);

    pcl::PointCloud<pcl::PointXYZI>::Ptr clustered_cloud(new pcl::PointCloud<pcl::PointXYZI>());
    auto cone_positions = clusterAndDetectCones(filtered_cloud, clustered_cloud);
    publishPointCloud(clustered_cloud, cluster_pub_);

    publishConePositions(cone_positions);
}

    pcl::PointCloud<pcl::PointXYZI>::Ptr extractROI(const pcl::PointCloud<pcl::PointXYZI>::Ptr &cloud)
    {
        float x_min = 0.8, x_max = 50.0;
        float y_min = -3.0, y_max = 3.0;
        float z_min = -1.1, z_max = 0.8;

        pcl::PassThrough<pcl::PointXYZI> pass;
        pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZI>());

        pass.setInputCloud(cloud);
        pass.setFilterFieldName("x");
        pass.setFilterLimits(x_min, x_max);
        pass.filter(*cloud_filtered);

        pass.setInputCloud(cloud_filtered);
        pass.setFilterFieldName("y");
        pass.setFilterLimits(y_min, y_max);
        pass.filter(*cloud_filtered);

        pass.setInputCloud(cloud_filtered);
        pass.setFilterFieldName("z");
        pass.setFilterLimits(z_min, z_max);
        pass.filter(*cloud_filtered);

        return cloud_filtered;
    }

    pcl::PointCloud<pcl::PointXYZI>::Ptr groundFilter(const pcl::PointCloud<pcl::PointXYZI>::Ptr &cloud)
    {
        pcl::SACSegmentation<pcl::PointXYZI> seg;
        pcl::PointIndices::Ptr inliers(new pcl::PointIndices());
        pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
        pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZI>());

        seg.setOptimizeCoefficients(true);
        seg.setModelType(pcl::SACMODEL_PLANE);
        seg.setMethodType(pcl::SAC_RANSAC);
        seg.setMaxIterations(90);
        seg.setDistanceThreshold(0.035);

        seg.setInputCloud(cloud);
        seg.segment(*inliers, *coefficients);

        pcl::ExtractIndices<pcl::PointXYZI> extract;
        extract.setInputCloud(cloud);
        extract.setIndices(inliers);
        extract.setNegative(true);
        extract.filter(*cloud_filtered);

        return cloud_filtered;
    }

    std::vector<Eigen::Vector4f> clusterAndDetectCones(
        const pcl::PointCloud<pcl::PointXYZI>::Ptr &cloud,
        pcl::PointCloud<pcl::PointXYZI>::Ptr &clustered_cloud)
    {
        std::vector<Eigen::Vector4f> cone_positions;
        if (cloud->points.empty()) return cone_positions;
    
        pcl::search::KdTree<pcl::PointXYZI>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZI>());
        tree->setInputCloud(cloud);
    
        std::vector<pcl::PointIndices> cluster_indices;
        pcl::EuclideanClusterExtraction<pcl::PointXYZI> ec;
        ec.setClusterTolerance(0.5);  // 调整聚类容差
        ec.setMinClusterSize(10);     // 设置最小点数
        ec.setMaxClusterSize(2000);    // 设置最大点数
        ec.setSearchMethod(tree);
        ec.setInputCloud(cloud);
        ec.extract(cluster_indices);
    
        // 定义几何特征阈值
        constexpr float MIN_HEIGHT = 0.1f;    // 最小高度
        constexpr float MAX_HEIGHT = 0.75f;    // 最大高度
        constexpr float MAX_WIDTH = 0.5f;     // 最大宽度
        constexpr float MAX_DEPTH = 0.5f;     // 最大深度
        constexpr float MIN_DENSITY = 0.1f;   // 最小密度（点数/体积）
    
        for (const auto &indices : cluster_indices)
        {
            pcl::PointCloud<pcl::PointXYZI>::Ptr cluster(new pcl::PointCloud<pcl::PointXYZI>());
            for (const auto &index : indices.indices)
                cluster->points.push_back(cloud->points[index]);
    
            // 计算点云簇的几何特征
            Eigen::Vector4f min_pt, max_pt;
            pcl::getMinMax3D(*cluster, min_pt, max_pt);
    
            float height = max_pt.z() - min_pt.z();       // 高度
            float width = max_pt.x() - min_pt.x();        // 宽度
            float depth = max_pt.y() - min_pt.y();        // 深度
            float volume = width * depth * height;        // 体积
            float density = cluster->points.size() / volume; // 密度
    
            // 过滤条件
            if (height >= MIN_HEIGHT && height <= MAX_HEIGHT && // 高度过滤
                width <= MAX_WIDTH && depth <= MAX_DEPTH &&    // 宽度和深度过滤
                density >= MIN_DENSITY)                        // 密度过滤
            {
                *clustered_cloud += *cluster;
    
                // 计算质心
                Eigen::Vector4f centroid;
                pcl::compute3DCentroid(*cluster, centroid);
                centroid[3] = cluster->points.size(); // 将第四个元素设为点云簇的数量
                cone_positions.push_back(centroid);
            }
        }
    
        return cone_positions;
    }
    

    void publishPointCloud(const pcl::PointCloud<pcl::PointXYZI>::Ptr &cloud,
                          const rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr &publisher)
    {
        sensor_msgs::msg::PointCloud2 output;
        pcl::toROSMsg(*cloud, output);
        output.header.stamp = this->now();
        output.header.frame_id = "hesai_lidar";
        publisher->publish(output);
    }

    void publishConePositions(const std::vector<Eigen::Vector4f> &cone_positions)
    {
        geometry_msgs::msg::PoseArray msg;
        msg.header.stamp = this->now();
        msg.header.frame_id = "hesai_lidar";

        for (const auto &pos : cone_positions)
        {
            geometry_msgs::msg::Pose pose;
            pose.position.x = pos[0];
            pose.position.y = pos[1];
            pose.position.z = pos[2];
            pose.orientation.w = pos[3]; // 存储点云数量
            msg.poses.push_back(pose);
        }
        cone_pub_->publish(msg);
    }

    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr sub_lidar;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr roi_pub_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr ground_pub_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr cluster_pub_;
    rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr cone_pub_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<HesaiProcessor>());
    rclcpp::shutdown();
    return 0;
}

