import rclpy
from rclpy.node import Node
from sensor_msgs.msg import PointCloud2
import sensor_msgs_py.point_cloud2 as pc2
import struct
from yolo_msgs.msg import DetectedObjectArray, DetectedObject
import numpy as np
import math

class MultiSensorFusionNode(Node):
    def __init__(self):
        super().__init__('multi_sensor_fusion_node')
        
        # 相机到雷达的变换矩阵（根据提供的参数）
        # 注意：lidar节点输出的坐标已经是相机坐标系，multi节点将其统一到雷达坐标系
        self.camera1_to_lidar = np.array([
            [0.112928,-0.993286,0.0244404,0.278646],
            [-0.134769,-0.0396847,-0.990101,0.735767],
            [0.984431,0.108577,-0.138317,1.80941],
            [0,0,0,1]
        ], dtype=np.float32)
        
        self.camera2_to_lidar = np.array([ 
            [-0.1064, -0.0693, 0.9919, -1.7252],
            [-0.9941, 0.0131, -0.1056, -0.3057],
            [-0.0057, -0.9975, -0.0703, 0.8431],
            [0, 0, 0, 1]], dtype=np.float32)
        
        # 创建检测结果订阅者
        self.detection_sub1 = self.create_subscription(
            DetectedObjectArray,
            '/detected_object_position1',
            self.detection_callback1,
            10
        )
        
        self.detection_sub2 = self.create_subscription(
            DetectedObjectArray,
            '/detected_object_position2',
            self.detection_callback2,
            10
        )
        
        # 创建点云订阅者
        self.pointcloud_sub1 = self.create_subscription(
            PointCloud2,
            '/detected_object_pointcloud1',
            self.pointcloud_callback1,
            10
        )
        
        self.pointcloud_sub2 = self.create_subscription(
            PointCloud2,
            '/detected_object_pointcloud2',
            self.pointcloud_callback2,
            10
        )
        
        # 创建发布者
        self.detection_pub = self.create_publisher(
            DetectedObjectArray,
            '/detected_objects_merged',
            10
        )
        
        self.pointcloud_pub = self.create_publisher(
            PointCloud2,
            '/detected_object_pointcloud_merged',
            10
        )

        # 缓存最新消息
        self.last_detection1 = None
        self.last_detection2 = None
        self.last_pointcloud1 = None
        self.last_pointcloud2 = None
        
        # 融合参数
        self.maha_threshold = 4.0          # 马氏距离门限
        self.iou_threshold = 0.1           # IOU门限
        
        self.get_logger().info('多传感器融合节点已启动（优化版）')

    # ==================== 综合权重融合核心方法 ====================
    
    def compute_mahalanobis_distance(self, obj_a, obj_b):
        """
        计算两个目标3D位置的马氏距离
        利用lidar节点已计算的 covariance_trace 作为不确定性度量
        """
        pos_a = np.array([obj_a.pose.position.x, obj_a.pose.position.y, obj_a.pose.position.z])
        pos_b = np.array([obj_b.pose.position.x, obj_b.pose.position.y, obj_b.pose.position.z])
        diff = pos_a - pos_b
        euclidean_dist_sq = np.sum(diff ** 2)
        
        # 直接从消息中读取协方差迹（lidar节点已计算）
        trace_a = getattr(obj_a, 'covariance_trace', 1.0) or 1.0
        trace_b = getattr(obj_b, 'covariance_trace', 1.0) or 1.0
        total_uncertainty = trace_a + trace_b
        
        if total_uncertainty < 1e-6:
            return float('inf')
        return math.sqrt(euclidean_dist_sq / total_uncertainty)

    def compute_comprehensive_weight(self, obj_a, obj_b, maha_dist):
        """
        综合权重：马氏代价 + IOU（来自lidar节点） + 协方差可靠性
        lidar节点已提供 iou_score 和 covariance_trace，无需重复计算单个目标的IOU
        """
        # 马氏距离权重因子
        if maha_dist < self.maha_threshold:
            maha_factor = math.exp(-maha_dist * maha_dist / (2.0 * self.maha_threshold))
        else:
            maha_factor = 0.0
        
        # 从消息直接读取（lidar节点已计算）
        iou_a = getattr(obj_a, 'iou_score', 0.5)
        iou_b = getattr(obj_b, 'iou_score', 0.5)
        score_a = getattr(obj_a, 'score', 0.5)
        score_b = getattr(obj_b, 'score', 0.5)
        trace_a = getattr(obj_a, 'covariance_trace', 10.0) or 10.0
        trace_b = getattr(obj_b, 'covariance_trace', 10.0) or 10.0
        
        # 协方差可靠性
        reference_trace = 10.0
        reliability_a = math.exp(-trace_a / reference_trace)
        reliability_b = math.exp(-trace_b / reference_trace)
        
        # 综合权重
        base_weight_a = score_a * iou_a
        base_weight_b = score_b * iou_b
        cov_factor = (reliability_a + reliability_b) * 0.5
        
        weight_a = base_weight_a * maha_factor * cov_factor
        weight_b = base_weight_b * maha_factor * cov_factor
        
        total_weight = weight_a + weight_b
        if total_weight < 1e-6:
            total_conf = score_a + score_b
            if total_conf < 1e-6:
                return 0.5, 0.5
            return score_a / total_conf, score_b / total_conf
        return weight_a / total_weight, weight_b / total_weight

    def compute_iou_between_objects(self, bbox_a, bbox_b):
        """计算两个边界框的IOU（两个目标间的重叠度，必须计算）"""
        xa1 = bbox_a.center.position.x - bbox_a.size.x * 0.5
        ya1 = bbox_a.center.position.y - bbox_a.size.y * 0.5
        xa2 = bbox_a.center.position.x + bbox_a.size.x * 0.5
        ya2 = bbox_a.center.position.y + bbox_a.size.y * 0.5
        
        xb1 = bbox_b.center.position.x - bbox_b.size.x * 0.5
        yb1 = bbox_b.center.position.y - bbox_b.size.y * 0.5
        xb2 = bbox_b.center.position.x + bbox_b.size.x * 0.5
        yb2 = bbox_b.center.position.y + bbox_b.size.y * 0.5
        
        xi1 = max(xa1, xb1)
        yi1 = max(ya1, yb1)
        xi2 = min(xa2, xb2)
        yi2 = min(ya2, yb2)
        w = xi2 - xi1
        h = yi2 - yi1
        if w <= 0 or h <= 0:
            return 0.0
        inter = w * h
        area_a = (xa2 - xa1) * (ya2 - ya1)
        area_b = (xb2 - xb1) * (yb2 - yb1)
        union = area_a + area_b - inter
        return inter / union if union > 0 else 0.0

    def comprehensive_fuse_objects(self, obj_a, obj_b):
        """综合权重融合，利用lidar节点已计算的数据"""
        maha_dist = self.compute_mahalanobis_distance(obj_a, obj_b)
        
        # 计算两个目标间的IOU（必要计算）
        iou_between = 0.0
        if hasattr(obj_a, 'bbox') and hasattr(obj_b, 'bbox'):
            iou_between = self.compute_iou_between_objects(obj_a.bbox, obj_b.bbox)
        
        maha_ok = maha_dist < self.maha_threshold
        iou_ok = iou_between > self.iou_threshold
        
        if not (maha_ok and iou_ok):
            score_a = getattr(obj_a, 'score', 0.5)
            score_b = getattr(obj_b, 'score', 0.5)
            return obj_a if score_a >= score_b else obj_b
        
        w_a, w_b = self.compute_comprehensive_weight(obj_a, obj_b, maha_dist)
        
        fused_x = w_a * obj_a.pose.position.x + w_b * obj_b.pose.position.x
        fused_y = w_a * obj_a.pose.position.y + w_b * obj_b.pose.position.y
        fused_z = w_a * obj_a.pose.position.z + w_b * obj_b.pose.position.z
        
        fused_obj = DetectedObject()
        fused_obj.header = obj_a.header
        fused_obj.track_id = getattr(obj_a, 'track_id', 0)
        fused_obj.class_id = getattr(obj_a, 'class_id', -1)
        fused_obj.class_name = getattr(obj_a, 'class_name', '')
        if hasattr(obj_a, 'bbox'):
            fused_obj.bbox = obj_a.bbox
        
        fused_obj.pose.position.x = fused_x
        fused_obj.pose.position.y = fused_y
        fused_obj.pose.position.z = fused_z
        fused_obj.pose.orientation.w = 1.0
        
        # 融合后的分数和协方差（加权平均）
        fused_obj.score = w_a * getattr(obj_a, 'score', 0.5) + w_b * getattr(obj_b, 'score', 0.5)
        fused_obj.covariance_trace = w_a * getattr(obj_a, 'covariance_trace', 0.0) + w_b * getattr(obj_b, 'covariance_trace', 0.0)
        fused_obj.iou_score = w_a * getattr(obj_a, 'iou_score', 0.5) + w_b * getattr(obj_b, 'iou_score', 0.5)
        
        return fused_obj

    # 保留原方法名兼容
    def dwf_fus_objects(self, obj_a, obj_b):
        return self.comprehensive_fuse_objects(obj_a, obj_b)

    def merge_detections(self, det1, det2):
        merged = DetectedObjectArray()
        merged.header = det1.header
        merged.header.frame_id = "lidar_link"
        
        objs1 = det1.objects
        objs2 = det2.objects
        if not objs1 and not objs2:
            return merged
        if not objs1:
            merged.objects.extend(objs2)
            return merged
        if not objs2:
            merged.objects.extend(objs1)
            return merged
        
        n1, n2 = len(objs1), len(objs2)
        cost_matrix = np.zeros((n1, n2), dtype=np.float32)
        for i, o1 in enumerate(objs1):
            for j, o2 in enumerate(objs2):
                maha = self.compute_mahalanobis_distance(o1, o2)
                iou_val = 0.0
                if hasattr(o1, 'bbox') and hasattr(o2, 'bbox'):
                    iou_val = self.compute_iou_between_objects(o1.bbox, o2.bbox)
                cost_matrix[i, j] = maha + (1.0 - iou_val) * self.maha_threshold
        
        # 贪心匹配
        matched_pairs = []
        used1, used2 = set(), set()
        costs = [(cost_matrix[i, j], i, j) for i in range(n1) for j in range(n2)]
        costs.sort(key=lambda x: x[0])
        for cost, i, j in costs:
            if i not in used1 and j not in used2:
                maha = self.compute_mahalanobis_distance(objs1[i], objs2[j])
                iou_val = 0.0
                if hasattr(objs1[i], 'bbox') and hasattr(objs2[j], 'bbox'):
                    iou_val = self.compute_iou_between_objects(objs1[i].bbox, objs2[j].bbox)
                if maha < self.maha_threshold and iou_val > self.iou_threshold:
                    matched_pairs.append((i, j))
                    used1.add(i)
                    used2.add(j)
        
        for i, j in matched_pairs:
            merged.objects.append(self.comprehensive_fuse_objects(objs1[i], objs2[j]))
        for i in range(n1):
            if i not in used1:
                merged.objects.append(objs1[i])
        for j in range(n2):
            if j not in used2:
                merged.objects.append(objs2[j])
        
        return merged

    # ==================== 回调与变换（优化点云批量变换） ====================
    
    def detection_callback1(self, msg):
        self.last_detection1 = self.transform_detections_to_lidar(msg, self.camera1_to_lidar)
        self.try_merge_and_publish()
    
    def detection_callback2(self, msg):
        self.last_detection2 = self.transform_detections_to_lidar(msg, self.camera2_to_lidar)
        self.try_merge_and_publish()
    
    def pointcloud_callback1(self, msg):
        self.last_pointcloud1 = self.transform_pointcloud_to_lidar_batch(msg, self.camera1_to_lidar)
        self.try_merge_and_publish()
    
    def pointcloud_callback2(self, msg):
        self.last_pointcloud2 = self.transform_pointcloud_to_lidar_batch(msg, self.camera2_to_lidar)
        self.try_merge_and_publish()
    
    def try_merge_and_publish(self):
        if self.last_detection1 is not None and self.last_detection2 is not None:
            merged_det = self.merge_detections(self.last_detection1, self.last_detection2)
            self.detection_pub.publish(merged_det)
            self.get_logger().info(f'发布合并检测: {len(merged_det.objects)} 个对象')
        
        if self.last_pointcloud1 is not None and self.last_pointcloud2 is not None:
            merged_pc = self.merge_pointclouds(self.last_pointcloud1, self.last_pointcloud2)
            self.pointcloud_pub.publish(merged_pc)
            self.get_logger().info(f'发布合并点云: {merged_pc.width} 个点')
    
    def transform_detections_to_lidar(self, detections_msg, T):
        """检测结果坐标变换（目标数量少，循环足够）"""
        transformed = DetectedObjectArray()
        transformed.header = detections_msg.header
        transformed.header.frame_id = "lidar_link"
        for obj in detections_msg.objects:
            p = np.array([obj.pose.position.x, obj.pose.position.y, obj.pose.position.z, 1.0])
            p_t = T @ p
            obj.pose.position.x = p_t[0]
            obj.pose.position.y = p_t[1]
            obj.pose.position.z = p_t[2]
            transformed.objects.append(obj)
        return transformed
    
    def transform_pointcloud_to_lidar_batch(self, cloud_msg, T):
        """
        批量点云坐标变换（性能优化）
        避免Python循环，使用numpy批量操作
        """
        # 读取点云为numpy数组 [N, 3]
        points_list = list(pc2.read_points(cloud_msg, field_names=("x", "y", "z"), skip_nans=True))
        if not points_list:
            return cloud_msg
        pts = np.array(points_list, dtype=np.float32)
        n = pts.shape[0]
        # 齐次坐标
        pts_h = np.hstack([pts, np.ones((n, 1), dtype=np.float32)])
        # 批量变换
        transformed = (T @ pts_h.T).T  # [N, 4]
        
        # 构建新的PointCloud2消息
        new_cloud = PointCloud2()
        new_cloud.header = cloud_msg.header
        new_cloud.header.frame_id = "lidar_link"
        new_cloud.height = 1
        new_cloud.width = n
        new_cloud.is_dense = False
        new_cloud.is_bigendian = False
        new_cloud.fields = [
            pc2.PointField(name='x', offset=0, datatype=pc2.PointField.FLOAT32, count=1),
            pc2.PointField(name='y', offset=4, datatype=pc2.PointField.FLOAT32, count=1),
            pc2.PointField(name='z', offset=8, datatype=pc2.PointField.FLOAT32, count=1),
        ]
        new_cloud.point_step = 12
        new_cloud.row_step = 12 * n
        # 打包数据
        new_cloud.data = struct.pack(f'{"fff"*n}', *transformed[:, :3].flatten())
        return new_cloud
    
    def merge_pointclouds(self, cloud1, cloud2):
        """合并两个点云（批量拼接）"""
        points1 = list(pc2.read_points(cloud1, field_names=("x", "y", "z"), skip_nans=True))
        points2 = list(pc2.read_points(cloud2, field_names=("x", "y", "z"), skip_nans=True))
        merged_pts = points1 + points2
        n = len(merged_pts)
        
        merged_cloud = PointCloud2()
        merged_cloud.header = cloud1.header
        merged_cloud.height = 1
        merged_cloud.width = n
        merged_cloud.is_dense = False
        merged_cloud.is_bigendian = False
        merged_cloud.fields = [
            pc2.PointField(name='x', offset=0, datatype=pc2.PointField.FLOAT32, count=1),
            pc2.PointField(name='y', offset=4, datatype=pc2.PointField.FLOAT32, count=1),
            pc2.PointField(name='z', offset=8, datatype=pc2.PointField.FLOAT32, count=1),
        ]
        merged_cloud.point_step = 12
        merged_cloud.row_step = 12 * n
        # 批量打包
        flat = []
        for p in merged_pts:
            flat.extend(p)
        merged_cloud.data = struct.pack(f'{"fff"*n}', *flat)
        return merged_cloud

def main(args=None):
    rclpy.init(args=args)
    node = MultiSensorFusionNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()