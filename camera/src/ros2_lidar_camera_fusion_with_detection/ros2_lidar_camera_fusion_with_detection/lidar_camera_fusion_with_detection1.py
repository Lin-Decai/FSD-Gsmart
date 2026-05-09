import rclpy
from rclpy.node import Node
import numpy as np
import yaml
import os
from ament_index_python.packages import get_package_share_directory
from sensor_msgs.msg import PointCloud2, CameraInfo, Image, PointField
import sensor_msgs_py.point_cloud2 as pc2
import math
import cv2
from cv_bridge import CvBridge
from yolo_msgs.msg import DetectionArray, DetectedObject, DetectedObjectArray
import struct
from scipy.optimize import linear_sum_assignment
from rclpy.qos import QoSProfile, QoSHistoryPolicy, QoSReliabilityPolicy, QoSDurabilityPolicy
import time

# ==================== 库引入完毕 ====================

class KalmanFilter:
    """
    ============================================================================
    6维卡尔曼滤波器 - 用于目标位置预测和平滑
    
    状态向量定义: X = [x, vx, ax, y, vy, ay]^T
        - x, y:     目标在图像平面或世界坐标系中的位置 (单位: 像素 或 米)
        - vx, vy:   目标在x和y方向的速度 (单位: 像素/秒 或 米/秒)
        - ax, ay:   目标在x和y方向的加速度 (单位: 像素/秒² 或 米/秒²)
    
    运动模型: 匀加速直线运动 (Constant Acceleration Model)
        - 假设目标在短时间内加速度恒定
        - 状态转移方程: X(k) = F * X(k-1) + w
          其中 w ~ N(0, Q) 是过程噪声
    
    观测模型: 只能直接观测位置
        - 观测方程: Z(k) = H * X(k) + v
          其中 v ~ N(0, R) 是观测噪声
        - Z = [x_measured, y_measured]^T
    
    卡尔曼滤波五步法:
        1. 预测状态: X_pred = F * X
        2. 预测协方差: P_pred = F * P * F^T + Q
        3. 计算卡尔曼增益: K = P_pred * H^T * (H * P_pred * H^T + R)^{-1}
        4. 更新状态: X = X_pred + K * (Z - H * X_pred)
        5. 更新协方差: P = (I - K * H) * P_pred
    ============================================================================
    """
    
    # __slots__ 用于限制实例属性，减少内存占用和属性访问时间
    # 每个实例只允许有这些属性，不能动态添加其他属性
    __slots__ = ('dt', 'x', 'P', 'F', 'H', 'Q', 'R', '_I')
    
    def __init__(self, dt=0.1, std_acc=1.0, std_meas=1.0):
        """
        初始化卡尔曼滤波器
        
        参数说明:
            dt: float - 采样时间间隔，即两帧之间的时间差 (单位: 秒)
                默认0.1表示假设10Hz的更新频率
                影响: dt越大，预测的不确定性越大，Q矩阵中元素越大
            
            std_acc: float - 加速度过程噪声标准差 (单位: 米/秒² 或 像素/秒²)
                含义: 目标实际运动与匀加速模型的偏离程度
                影响: std_acc越小 → Q矩阵越小 → 更相信运动模型 → 轨迹更平滑但响应慢
                      std_acc越大 → Q矩阵越大 → 更相信测量值 → 响应快但噪声大
            
            std_meas: float - 测量噪声标准差 (单位: 米 或 像素)
                含义: 传感器测量值的不确定性
                影响: std_meas越小 → R矩阵越小 → 更相信测量值 → 响应快但可能抖动
                      std_meas越大 → R矩阵越大 → 更相信模型预测 → 轨迹平滑但滞后
        """
        # dt: 采样时间间隔
        self.dt = dt
        
        # x: 状态向量，形状 (6,)，dtype=float32 节省内存并加速计算
        # 初始化为零向量，表示目标从原点开始，速度和加速度为零
        self.x = np.zeros(6, dtype=np.float32)
        
        # P: 状态协方差矩阵，形状 (6, 6)
        # 表示状态估计的不确定性，初始化为对角阵*10表示初始不确定性较大
        # P[i,i]越大，表示第i个状态量的估计越不可靠
        # 初始化为10.0是因为刚开始跟踪时完全不知道目标状态
        self.P = np.eye(6, dtype=np.float32) * 10.0
        
        # F: 状态转移矩阵，形状 (6, 6)
        # 描述状态如何从k-1时刻演变到k时刻: X(k) = F * X(k-1)
        # 
        # 推导过程 (以x方向为例):
        #   位置: x(k) = x(k-1) + vx(k-1)*dt + 0.5*ax(k-1)*dt²
        #   速度: vx(k) = vx(k-1) + ax(k-1)*dt
        #   加速度: ax(k) = ax(k-1)  (假设加速度恒定)
        #
        # 矩阵形式:
        #   [x ]   [1, dt, 0.5*dt², 0,  0,     0]   [x ]
        #   [vx]   [0,  1,      dt, 0,  0,     0]   [vx]
        #   [ax] = [0,  0,       1, 0,  0,     0] * [ax]
        #   [y ]   [0,  0,       0, 1, dt, 0.5*dt²] [y ]
        #   [vy]   [0,  0,       0, 0,  1,    dt]   [vy]
        #   [ay]   [0,  0,       0, 0,  0,     1]   [ay]
        dt2 = 0.5 * dt * dt  # 0.5 * dt²，用于位置更新的加速度项
        self.F = np.array([
            [1, dt, dt2, 0,  0,   0],  # x 位置更新: x + vx*dt + 0.5*ax*dt²
            [0,  1, dt,  0,  0,   0],  # vx 速度更新: vx + ax*dt
            [0,  0,  1,  0,  0,   0],  # ax 加速度保持不变
            [0,  0,  0,  1, dt, dt2],  # y 位置更新: y + vy*dt + 0.5*ay*dt²
            [0,  0,  0,  0,  1,  dt],  # vy 速度更新: vy + ay*dt
            [0,  0,  0,  0,  0,   1]   # ay 加速度保持不变
        ], dtype=np.float32)
        
        # H: 观测矩阵，形状 (2, 6)
        # 描述状态向量如何映射到观测值: Z = H * X
        # 我们只能直接观测位置 (x, y)，不能直接观测速度和加速度
        # 所以H提取状态向量的第0和第3个元素
        self.H = np.array([
            [1, 0, 0, 0, 0, 0],  # 观测x: 取状态向量第0个元素
            [0, 0, 0, 1, 0, 0]   # 观测y: 取状态向量第3个元素
        ], dtype=np.float32)
        
        # Q: 过程噪声协方差矩阵，形状 (6, 6)
        # 表示运动模型的不确定性，假设噪声来自随机的加速度变化
        # 
        # 推导: 假设加速度的随机变化服从正态分布 N(0, std_acc²)
        # 这种随机加速度会在dt时间内对位置和速度产生累积影响
        # 
        # 对于连续时间白噪声加速度模型，离散化后的Q矩阵为:
        # Q = G * (std_acc² * I) * G^T
        # 其中G是噪声输入矩阵: G = [0.5*dt², dt, 1]^T (对每个方向)

        dt2 = dt ** 2   # dt²
        dt3 = dt ** 3   # dt³
        dt4 = dt ** 4   # dt⁴
        dt5 = dt ** 5   # dt^5
        q = std_acc ** 2  # 加速度噪声方差
        
        self.Q = q * np.array([
            # x方向噪声
            [dt5/20,dt4/8,dt3/6,0,0,0],
            [dt4/8,dt3/3,dt2/2,0,0,0],
            [dt3/6,dt2/2,dt,0,0,0],
            # y方向噪声 (与x方向对称)
            [0,0,0,dt5/20,dt4/8,dt3/6],
            [0,0,0,dt4/8,dt3/3,dt2/2],
            [0,0,0,dt3/6,dt2/2,dt]
        ], dtype=np.float32)
        
        # R: 观测噪声协方差矩阵，形状 (2, 2)
        # 表示传感器测量值的不确定性
        # 假设x和y方向的测量噪声相互独立且方差相同
        # 对角阵，每个元素都是 std_meas²
        self.R = np.eye(2, dtype=np.float32) * (std_meas ** 2)
        
        # _I: 6x6单位矩阵，用于更新步骤的计算
        # 预存避免重复创建，提高效率
        self._I = np.eye(6, dtype=np.float32)
    
    def predict(self):
        """
        卡尔曼滤波预测步骤
        
        计算公式:
            1. X_pred = F * X           (状态预测)
            2. P_pred = F * P * F^T + Q (协方差预测)
        
        返回:
            (float, float): 预测的目标位置 (x, y)
            
        注意: 预测后状态不确定性增加 (P变大)，因为运动模型不完美
        """
        # 状态预测: X = F * X
        # 根据运动模型将状态向前推进一步
        self.x = self.F @ self.x
        
        # 协方差预测: P = F * P * F^T + Q
        # F * P * F^T 表示原有不确定性通过运动模型的传播
        # + Q 表示加入运动模型本身的不确定性
        self.P = self.F @ self.P @ self.F.T + self.Q
        
        # 返回预测的位置
        return float(self.x[0]), float(self.x[3])
    
    def update(self, z):
        """
        卡尔曼滤波更新步骤 - 用新的测量值修正预测
        
        参数:
            z: list或tuple - 测量值 [x_measured, y_measured]
        
        计算公式:
            3. y = Z - H * X                    (计算残差/新息)
            4. S = H * P * H^T + R              (残差协方差)
            5. K = P * H^T * S^{-1}             (卡尔曼增益)
            6. X = X + K * y                    (状态更新)
            7. P = (I - K * H) * P              (协方差更新)
        
        返回:
            (float, float): 更新后的目标位置 (x, y)
        """
        # 将测量值转换为列向量 [2, 1]
        z = np.asarray(z, dtype=np.float32).reshape(2, 1)
        
        # 状态向量也需要转为列向量进行矩阵运算
        x_col = self.x.reshape(6, 1)
        
        # y: 残差 (innovation) = Z - H * X，形状 [2, 1]
        # 表示测量值与预测值的差异
        # y越大说明预测越不准，或者测量噪声越大
        y = z - self.H @ x_col
        
        # P_H_T: P * H^T，中间变量用于后续计算，形状 [6, 2]
        P_H_T = self.P @ self.H.T
        
        # S: 残差协方差矩阵 = H * P * H^T + R，形状 [2, 2]
        # 表示残差y的不确定性
        # H * P * H^T 是预测不确定性在观测空间的投影
        # R 是测量噪声
        S = self.H @ P_H_T + self.R
        
        # 数值稳定性: 给S的对角线加一个小值防止矩阵奇异
        # 当P很小时可能出现数值问题
        S[0, 0] += 1e-6
        S[1, 1] += 1e-6
        
        # K: 卡尔曼增益矩阵，形状 [6, 2]
        # K = P * H^T * S^{-1}
        # 
        # 卡尔曼增益决定了更相信预测还是更相信测量:
        #   - 如果R很小(测量准确) → S主要由R决定 → K变大 → 更相信测量
        #   - 如果P很小(预测准确) → S主要由H*P*H^T决定 → K变小 → 更相信预测
        K = P_H_T @ np.linalg.inv(S)
        
        # 状态更新: X = X + K * y
        # K * y 是对预测状态的修正量
        # 修正量 = 卡尔曼增益 * 残差
        self.x = (x_col + K @ y).flatten()
        
        # 协方差更新: P = (I - K * H) * P
        # 注意: 标准公式是 (I - K*H) * P，也可以写成 P - K*H*P
        # 由于加入了测量信息，状态不确定性减小
        self.P = (self._I - K @ self.H) @ self.P
        
        # 返回更新后的位置
        return float(self.x[0]), float(self.x[3])
    
    def get_position(self):
        """
        返回当前估计的位置
        
        返回:
            (float, float): 当前估计的 (x, y) 位置
        """
        return float(self.x[0]), float(self.x[3])


class Track:
    """
    ============================================================================
    单个目标跟踪器 - 管理一个目标的完整跟踪状态
    
    每个Track包含:
        - 一个唯一的跟踪ID
        - 一个专用的卡尔曼滤波器用于位置平滑
        - 当前的边界框
        - 跟踪状态信息 (更新时间、匹配分数等)
    
    生命周期:
        1. 创建: 当新的检测框无法匹配任何现有跟踪器时
        2. 更新: 当检测框成功匹配时，用新检测框更新状态
        3. 预测: 每一帧都执行，即使没有匹配的检测框
        4. 删除: 当连续多帧没有被更新时 (time_since_update >= max_age)
    ============================================================================
    """
    
    # __slots__ 限制实例属性，每个Track实例节省约1KB内存
    __slots__ = ('track_id', 'kf', 'bbox', 'time_since_update', 'max_age',
                 'iou_score', 'mahalanobis', 'covariance_trace')
    
    def __init__(self, track_id, bbox, dt=0.1, std_acc=0.5, std_meas=0.1):
        """
        初始化一个新的跟踪器
        
        参数:
            track_id: int - 唯一的跟踪标识符，从1开始递增
            bbox: tuple/list - 边界框 [x1, y1, x2, y2]
                x1, y1: 左上角坐标
                x2, y2: 右下角坐标
            dt: float - 卡尔曼滤波器的采样时间间隔 (秒)
            std_acc: float - 加速度过程噪声标准差
                后期要调参: 针对锥桶等小目标，可能需要调小以减少过冲
            std_meas: float - 测量噪声标准差
                后期要调参: 根据检测器的稳定性调整
        """
        # track_id: 目标唯一标识符，用于跨帧关联同一个目标
        self.track_id = track_id
        
        # kf: 该目标专属的卡尔曼滤波器
        # 每个目标独立滤波，因为不同目标的运动模式不同
        # 后期要调参: std_acc和std_meas需要根据实际场景调整
        self.kf = KalmanFilter(dt=dt, std_acc=std_acc, std_meas=std_meas)
        
        # bbox: 当前边界框 (x1, y1, x2, y2)
        # 转为tuple使其不可变，防止意外修改
        self.bbox = tuple(bbox)
        
        # time_since_update: 距离上次成功匹配的帧数
        # 每次predict时+1，update时重置为0
        # 用于判断目标是否已经消失
        self.time_since_update = 0
        
        # max_age: 最大允许的丢失帧数
        # 超过此值认为目标永久消失，从跟踪列表中删除
        # 30帧 × 0.1秒/帧 = 3秒，即目标消失3秒后放弃跟踪
        self.max_age = 30
        
        # iou_score: 最近一次匹配的IOU分数
        # 范围[0, 1]，表示预测框和检测框的重叠程度
        self.iou_score = 0.0
        
        # mahalanobis: 最近一次匹配的马氏距离
        # 考虑了状态不确定性的统计距离
        self.mahalanobis = 0.0
        
        # covariance_trace: 卡尔曼协方差矩阵的迹
        # trace(P) = sum(P[i,i])，表示总体估计不确定性
        # 值越大表示状态估计越不可靠
        self.covariance_trace = float(np.trace(self.kf.P))
        
        # 用初始检测框初始化卡尔曼状态
        # 计算边界框中心点作为初始位置
        cx = (bbox[0] + bbox[2]) * 0.5  # 中心x坐标
        cy = (bbox[1] + bbox[3]) * 0.5  # 中心y坐标
        
        # 初始化状态向量中的位置分量
        # 速度和加速度保持为0 (初始假设目标静止)
        self.kf.x[0] = cx  # x位置
        self.kf.x[3] = cy  # y位置
    
    def predict(self):
        """
        预测下一帧的目标位置
        
        调用卡尔曼滤波器的predict方法，然后用预测的位置更新边界框
        边界框的宽高保持不变 (假设目标大小不变)
        """
        # 执行卡尔曼预测，获得预测的中心点位置
        self.kf.predict()
        
        # 获取预测后的中心点坐标
        cx, cy = self.kf.get_position()
        
        # 计算当前边界框的宽高
        w = self.bbox[2] - self.bbox[0]  # 宽度 = x2 - x1
        h = self.bbox[3] - self.bbox[1]  # 高度 = y2 - y1
        
        # 用新的中心点更新边界框，保持宽高不变
        # 假设: 目标的形状在短时间内不会发生显著变化
        self.bbox = (
            cx - w * 0.5,  # 新的x1
            cy - h * 0.5,  # 新的y1
            cx + w * 0.5,  # 新的x2
            cy + h * 0.5   # 新的y2
        )
        
        # 增加未更新计数
        self.time_since_update += 1
    
    def update(self, bbox):
        """
        用新的检测框更新跟踪器
        
        参数:
            bbox: tuple/list - 新的检测框 [x1, y1, x2, y2]
        """
        # 计算新检测框的中心点
        cx = (bbox[0] + bbox[2]) * 0.5
        cy = (bbox[1] + bbox[3]) * 0.5
        
        # 用中心点作为测量值，执行卡尔曼更新
        # 这会对位置进行平滑，减少检测噪声的影响
        self.kf.update([cx, cy])
        
        # 直接使用检测框作为当前边界框
        # 注意: 这里没有使用平滑后的位置重建边界框
        # 因为检测框的宽高可能更准确
        self.bbox = tuple(bbox)
        
        # 重置未更新计数
        self.time_since_update = 0


class MultiTargetTracker:
    """
    ============================================================================
    多目标跟踪管理器 - 使用匈牙利算法进行数据关联
    
    核心算法流程:
        1. 预测: 所有现有跟踪器预测下一帧位置
        2. 关联: 构建代价矩阵，使用匈牙利算法找最优匹配
        3. 门限筛选: 用马氏距离和IOU双重门限过滤错误匹配
        4. 更新: 匹配成功的跟踪器用检测框更新
        5. 管理: 删除超时跟踪器，为新检测创建跟踪器
    
    数据关联原理:
        问题: 给定m个跟踪器和n个检测框，如何找到最佳的一对一匹配?
        方法: 
            1. 计算每对(跟踪器, 检测框)的匹配代价 (这里用马氏距离)
            2. 构建m×n代价矩阵
            3. 使用匈牙利算法找到总代价最小的匹配方案
            4. 用门限去除代价过高的匹配 (防止错误关联)
    ============================================================================
    """
    
    def __init__(self, maha_threshold=7.0, iou_threshold=0.1):
        """
        初始化多目标跟踪器
        
        参数:
            maha_threshold: float - 马氏距离门限值
                马氏距离服从卡方分布，对于2自由度:
                - 门限5.99对应95%置信区间
                - 门限7.38对应97.5%置信区间
                - 门限9.21对应99%置信区间
                建议值: 5~9之间，小目标锥筒用稍小的值
                后期要调参: 根据检测器精度和场景复杂度调整
            
            iou_threshold: float - IOU门限值
                IOU = 交集面积 / 并集面积，范围[0, 1]
                用于二次确认匹配的有效性
                小目标锥筒用0.05~0.1，因为框小容易错位
                后期要调参: 根据目标大小调整，目标越小阈值越低
        """
        # tracks: 当前活跃的跟踪器列表
        self.tracks = []
        
        # next_id: 下一个可用的跟踪ID
        # 每个新目标分配一个唯一ID，从1开始递增
        self.next_id = 1
        
        # maha_threshold: 马氏距离门限
        # 超过此值的匹配被认为不可靠而被拒绝
        self.maha_threshold = maha_threshold
        
        # iou_threshold: IOU门限
        # IOU低于此值的匹配被拒绝
        self.iou_threshold = iou_threshold
    
    @staticmethod
    def iou(a, b):
        """
        计算两个边界框的IOU (Intersection over Union)
        
        IOU = 交集面积 / 并集面积
        
        参数:
            a, b: 边界框 [x1, y1, x2, y2]
                x1, y1: 左上角
                x2, y2: 右下角
        
        返回:
            float: IOU值，范围[0, 1]
                0: 完全不相交
                1: 完全重合
        
        计算公式:
            intersection = max(0, min(xa2, xb2) - max(xa1, xb1)) 
                         * max(0, min(ya2, yb2) - max(ya1, yb1))
            union = area_a + area_b - intersection
            IOU = intersection / union
        """
        # 解包边界框坐标
        xa1, ya1, xa2, ya2 = a
        xb1, yb1, xb2, yb2 = b
        
        # 计算交集区域的左上角和右下角
        # 交集左上角 = 两个框左上角的最大值
        xi1 = max(xa1, xb1)
        yi1 = max(ya1, yb1)
        # 交集右下角 = 两个框右下角的最小值
        xi2 = min(xa2, xb2)
        yi2 = min(ya2, yb2)
        
        # 计算交集宽高，如果为负数表示不相交，取0
        w = xi2 - xi1
        h = yi2 - yi1
        if w <= 0 or h <= 0:
            return 0.0  # 没有交集
        
        # 交集面积
        inter = w * h
        
        # 分别计算两个框的面积
        area_a = (xa2 - xa1) * (ya2 - ya1)
        area_b = (xb2 - xb1) * (yb2 - yb1)
        
        # 并集面积 = 面积A + 面积B - 交集面积
        union = area_a + area_b - inter
        
        # 返回IOU，防止除以0
        return inter / union if union > 0 else 0.0
    
    def update(self, detections):
        """
        用新的检测框更新所有跟踪器
        
        参数:
            detections: list - 检测框列表，每个元素为 [x1, y1, x2, y2]
        
        返回:
            list: 跟踪结果，每个元素为 (track_id, bbox)
        """
        # ========== 边界情况: 无检测框 ==========
        if not detections:
            # 即使没有检测，也要让所有跟踪器执行预测
            for t in self.tracks:
                t.predict()
            
            # 删除超时的跟踪器
            self.tracks = [t for t in self.tracks if t.time_since_update < t.max_age]
            
            # 返回当前跟踪结果
            return [(t.track_id, t.bbox) for t in self.tracks]
        
        num_tracks = len(self.tracks)
        num_dets = len(detections)
        
        # ========== 步骤1: 所有跟踪器执行预测 ==========
        for t in self.tracks:
            t.predict()
        
        # ========== 步骤2: 构建代价矩阵并进行匈牙利匹配 ==========
        if num_tracks > 0:
            # cost_matrix: 代价矩阵，形状 [num_tracks, num_dets]
            # cost_matrix[i, j] = 跟踪器i与检测框j的马氏距离
            # 距离越小表示匹配越好
            cost_matrix = np.zeros((num_tracks, num_dets), dtype=np.float32)
            
            # 计算每个(跟踪器, 检测框)对的马氏距离
            for ti, t in enumerate(self.tracks):
                # 获取跟踪器预测的中心点位置
                cx_pred, cy_pred = t.kf.get_position()
                
                # S: 残差协方差矩阵 = H * P * H^T + R
                # 表示预测位置的不确定性
                S = t.kf.H @ t.kf.P @ t.kf.H.T + t.kf.R
                
                # inv_S: S的逆矩阵，用于计算马氏距离
                # 加小量防止奇异
                inv_S = np.linalg.inv(S + np.eye(2) * 1e-8)
                
                for di, det in enumerate(detections):
                    # 检测框中心点
                    cx_det = (det[0] + det[2]) * 0.5
                    cy_det = (det[1] + det[3]) * 0.5
                    
                    # diff: 检测位置与预测位置的差值向量 [dx, dy]
                    diff = np.array([cx_det - cx_pred, cy_det - cy_pred])
                    
                    # 马氏距离 = sqrt(diff^T * inv_S * diff)
                    # 
                    # 马氏距离 vs 欧氏距离:
                    #   欧氏距离: 假设各方向不确定性相同且独立
                    #   马氏距离: 考虑了协方差，能反映方向性的不确定性
                    #   例如: 如果x方向不确定性大，则x方向的差异会被"缩小"
                    cost_matrix[ti, di] = np.sqrt(diff @ inv_S @ diff)
            
            # 匈牙利算法: 找到总代价最小的二分图匹配
            # linear_sum_assignment返回 (行索引, 列索引) 的最优匹配
            t_indices, d_indices = linear_sum_assignment(cost_matrix)
            
            # ========== 步骤3: 双重门限筛选有效匹配 ==========
            matches = []           # 有效匹配列表
            matched_tracks = set()  # 已匹配的跟踪器索引
            matched_dets = set()    # 已匹配的检测框索引
            
            for ti, di in zip(t_indices, d_indices):
                # 门限1: 马氏距离检查
                maha_ok = cost_matrix[ti, di] < self.maha_threshold
                
                # 门限2: IOU检查
                # 计算预测框与检测框的IOU
                iou_val = self.iou(self.tracks[ti].bbox, detections[di])
                iou_ok = iou_val > self.iou_threshold
                
                # 只有两个门限都通过才算有效匹配
                if maha_ok and iou_ok:
                    matches.append((ti, di))
                    matched_tracks.add(ti)
                    matched_dets.add(di)
            
            # ========== 步骤4: 更新匹配成功的跟踪器 ==========
            for ti, di in matches:
                # 用检测框更新跟踪器
                self.tracks[ti].update(detections[di])
                
                # 记录匹配质量指标
                self.tracks[ti].iou_score = self.iou(self.tracks[ti].bbox, detections[di])
                self.tracks[ti].covariance_trace = float(np.trace(self.tracks[ti].kf.P))
            
            # ========== 步骤5: 为未匹配的检测创建新跟踪器 ==========
            for di in range(num_dets):
                if di not in matched_dets:
                    # 新目标出现，创建新的跟踪器
                    self.tracks.append(Track(self.next_id, detections[di]))
                    self.next_id += 1
        else:
            # 没有现有跟踪器，所有检测都创建新跟踪器
            for det in detections:
                self.tracks.append(Track(self.next_id, det))
                self.next_id += 1
        
        # ========== 步骤6: 删除超时丢失的跟踪器 ==========
        # 只保留未更新帧数小于max_age的跟踪器
        self.tracks = [t for t in self.tracks if t.time_since_update < t.max_age]
        
        # ========== 步骤7: 返回跟踪结果 ==========
        return [(t.track_id, t.bbox) for t in self.tracks]


class PointCloudProjector:
    """
    ============================================================================
    点云投影器 - 将激光雷达3D点云投影到2D图像平面
    
    工作原理:
        1. 坐标系变换: 激光雷达坐标系 → 相机坐标系
           使用外参矩阵 (旋转+平移) 进行刚体变换
           
        2. 透视投影: 相机坐标系3D点 → 图像平面2D点
           使用相机内参 (焦距+主点) 进行投影
           
    数学公式:
        [u]   [fx  0  cx]   [Xc]
        [v] = [0  fy  cy] * [Yc] / Zc
        [1]   [0   0   1]   [Zc]
        
        其中:
            (Xc, Yc, Zc) = T * (Xl, Yl, Zl, 1)^T
            T是4x4变换矩阵 (激光雷达到相机)
            (u, v)是图像坐标
    ============================================================================
    """
    
    def __init__(self, transformation_matrix, fx, fy, cx, cy, image_width, image_height):
        """
        初始化点云投影器
        
        参数:
            transformation_matrix: np.ndarray - 4x4变换矩阵 (激光雷达到相机)
                形式: [R|t]
                      [0|1]
                其中R是3x3旋转矩阵，t是3x1平移向量
                
            fx, fy: float - 相机焦距 (像素单位)
                表示镜头将3D点投影到成像平面的缩放因子
                fx = f * sx, fy = f * sy
                其中f是物理焦距，sx,sy是像素密度
                
            cx, cy: float - 主点坐标 (像素单位)
                光轴与图像平面的交点，通常是图像中心
                
            image_width, image_height: int - 图像尺寸 (像素)
                用于边界检查，超出范围的投影点被丢弃
        """
        # T: 4x4变换矩阵，激光雷达坐标系 → 相机坐标系
        self.T = np.asarray(transformation_matrix, dtype=np.float32)
        
        # 相机内参
        self.fx = fx  # x方向焦距 (像素)
        self.fy = fy  # y方向焦距 (像素)
        self.cx = cx  # 主点x坐标
        self.cy = cy  # 主点y坐标
        
        # 图像尺寸
        self.width = image_width
        self.height = image_height
    
    def project(self, x, y, z):
        """
        将单个点云投影到图像平面 (逐点版本)
        
        参数:
            x, y, z: float - 激光雷达坐标系下的点坐标 (米)
        
        返回:
            tuple: (transformed_point, u, v)
                transformed_point: 相机坐标系下的点 (Xc, Yc, Zc) 或 None
                u, v: 图像坐标 (像素)，无效时为-1
        """
        # 步骤1: 坐标系变换 (激光雷达 → 相机)
        # 齐次坐标变换: [Xc, Yc, Zc, 1]^T = T * [x, y, z, 1]^T
        tx = self.T[0,0]*x + self.T[0,1]*y + self.T[0,2]*z + self.T[0,3]  # Xc
        ty = self.T[1,0]*x + self.T[1,1]*y + self.T[1,2]*z + self.T[1,3]  # Yc
        tz = self.T[2,0]*x + self.T[2,1]*y + self.T[2,2]*z + self.T[2,3]  # Zc
        
        # 深度检查: Zc <= 0 表示点在相机后方，无法投影
        if tz <= 0:
            return None, -1, -1
        
        # 步骤2: 透视投影 (相机坐标系 → 图像平面)
        # 透视除法: 除以深度Zc
        inv_z = 1.0 / tz
        
        # 投影公式: u = fx * Xc/Zc + cx, v = fy * Yc/Zc + cy
        u = int(self.fx * tx * inv_z + self.cx)
        v = int(self.fy * ty * inv_z + self.cy)
        
        # 步骤3: 边界检查
        if 0 <= u < self.width and 0 <= v < self.height:
            return (tx, ty, tz), u, v
        
        return None, u, v
    
    def project_batch(self, points):
        """
        批量投影点云 - 使用向量化计算大幅提升效率
        
        参数:
            points: list - 点云列表，每个元素为 [x, y, z]
        
        返回:
            tuple: (transformed_points, u_arr, v_arr)
                transformed_points: 相机坐标系下的有效点
                u_arr, v_arr: 对应的图像坐标
                
        效率对比:
            逐点循环: ~1000点/毫秒
            批量向量化: ~100000点/毫秒 (快100倍)
        """
        if not points:
            return [], [], []
        
        # 转换为numpy数组，形状 [n, 3]
        pts = np.asarray(points, dtype=np.float32)
        n = len(pts)
        
        # 构造齐次坐标: 添加一列1，形状 [n, 4]
        ones = np.ones((n, 1), dtype=np.float32)
        pts_h = np.hstack([pts, ones])
        
        # 批量坐标系变换: [n, 4] @ [4, 4].T = [n, 4]
        # 结果每行是 [Xc, Yc, Zc, 1]
        transformed = pts_h @ self.T.T
        
        # 提取变换后的坐标
        tx = transformed[:, 0]  # Xc
        ty = transformed[:, 1]  # Yc
        tz = transformed[:, 2]  # Zc
        
        # 筛选有效深度: Zc > 0
        valid = tz > 0
        if not np.any(valid):
            return [], [], []
        
        # 计算透视投影 (只对有效点)
        inv_z = 1.0 / tz[valid]  # 1/Zc
        
        # 批量投影: u = fx * Xc/Zc + cx, v = fy * Yc/Zc + cy
        u = (self.fx * tx[valid] * inv_z + self.cx).astype(np.int32)
        v = (self.fy * ty[valid] * inv_z + self.cy).astype(np.int32)
        
        # 图像边界筛选
        in_bounds = (u >= 0) & (u < self.width) & (v >= 0) & (v < self.height)
        
        # 返回有效的结果
        transformed_points = transformed[valid][in_bounds]  # 相机坐标系点
        u_valid = u[in_bounds]  # 图像u坐标
        v_valid = v[in_bounds]  # 图像v坐标
        
        return transformed_points, u_valid, v_valid


class LidarCameraFusionNode(Node):
    """
    ============================================================================
    激光雷达与相机融合节点 - 精简高效版
    
    功能: 将激光雷达点云与YOLO目标检测结果融合，输出目标的3D位置
    
    数据流:
        输入订阅:
            - /hesai/roi_cloud: 激光雷达点云 (PointCloud2)
            - camera_1/camera_info_1: 相机内参 (CameraInfo)
            - /camera_1/image_raw_1: 相机图像 (Image)
            - detections: YOLO检测结果 (DetectionArray)
            
        输出发布:
            - /image_lidar1: 标注了投影点的图像 (Image)
            - /detected_object_position1: 检测目标的3D位置 (DetectedObjectArray)
            - /detected_object_pointcloud1: 每个目标的点云 (PointCloud2)
            
    处理流程:
        1. 接收YOLO检测框 → 多目标跟踪 → 获得稳定的跟踪框和ID
        2. 接收点云 → 批量投影到图像平面
        3. 判断投影点落在哪个跟踪框内 → 收集框内点云
        4. 对框内点云取中位数 → 得到目标的3D位置
        5. 卡尔曼滤波平滑位置 → 发布结果
    ============================================================================
    """
    
    def __init__(self):
        super().__init__('lidar_to_image_projection1')
        
        # ==================== 加载配置文件 ====================
        # 获取功能包共享目录路径
        package_share_directory = get_package_share_directory('ros2_lidar_camera_fusion_with_detection')
        
        # 构建配置文件完整路径
        yaml_file_path = os.path.join(package_share_directory, 'config', 'setup_config1.yaml')
        
        # 加载YAML配置
        yaml_data = self.load_yaml_file(yaml_file_path)
        
        # transformation_matrix: 4x4激光雷达到相机的变换矩阵
        # 包含旋转和平移信息
        self.transformation_matrix = np.array(yaml_data['transformation_matrix'], dtype=np.float32)
        
        # depth_range_min/max: 有效点云深度范围 (米)
        # 只处理在此范围内的点云，过滤掉太近或太远的点
        self.depth_range_min = yaml_data['depth_range']['min']
        self.depth_range_max = yaml_data['depth_range']['max']
        
        # 日志输出配置信息
        self.get_logger().info(f'Loaded transformation matrix:\n{self.transformation_matrix}')
        self.get_logger().info(f'Loaded depth_range: min={self.depth_range_min}, max={self.depth_range_max}')
        self.get_logger().info('Lidar & Camera Fusion Node has been started.')
        
        # ==================== QoS配置 ====================
        # 检测结果的QoS配置: 可靠传输、非持久化
        self.detect_qos = QoSProfile(
            reliability=QoSReliabilityPolicy.RELIABLE,  # 确保消息不丢失
            durability=QoSDurabilityPolicy.VOLATILE,    # 不持久化存储
            history=QoSHistoryPolicy.KEEP_LAST,          
            depth=10                                     # 队列深度
        )
        
        # ==================== 初始化变量 ====================
        # current_image: 当前帧图像 (OpenCV BGR格式)
        self.current_image = None
        
        # bridge: ROS图像消息与OpenCV格式的转换器
        self.bridge = CvBridge()
        
        # 相机内参 (从CameraInfo消息获取)
        self.fx = None  # x方向焦距
        self.fy = None  # y方向焦距
        self.cx = None  # 主点x坐标
        self.cy = None  # 主点y坐标
        
        # 图像尺寸 (默认1080P，会从消息更新)
        self.image_width = 1920
        self.image_height = 1080
        
        # fusion: 多目标跟踪器
        # 用于将帧间检测框关联起来，分配稳定ID
        # 参数说明见MultiTargetTracker类的注释
        self.fusion = MultiTargetTracker(
            maha_threshold=7.0,   # 马氏距离门限，后期要调参
            iou_threshold=0.1     # IOU门限，小目标用较低值
        )
        
        # detections_cache: 缓存处理后的检测结果
        # 每个元素为字典，包含: bbox, track_id, class_id, class_name, score, cx, cy
        self.detections_cache = []
        
        # _detection_lock: 简单的线程安全标志
        # 防止在处理检测回调时被新检测打断
        self._detection_lock = False
        
        # projector: 点云投影器，收到相机内参后初始化
        self.projector = None
        
        # kf_lidar: 激光雷达坐标的卡尔曼滤波器
        # 用于平滑从点云计算出的3D位置
        # std_acc=0.3: 假设目标在3D空间中的加速度变化较小
        # std_meas=0.1: 点云测量有约10cm的噪声
        self.kf_lidar = KalmanFilter(dt=0.1, std_acc=0.3, std_meas=0.1)
        
        # ==================== 订阅器 ====================
        # 点云订阅 - 激光雷达ROI区域点云
        self.subscription = self.create_subscription(
            PointCloud2, '/hesai/roi_cloud', self.pointcloud_callback, 10)
        
        # 相机信息订阅 - 获取内参
        self.camera_info_subscription = self.create_subscription(
            CameraInfo, 'camera_1/camera_info_1', self.camera_info_callback, 10)
        
        # 图像订阅 - 原始相机图像
        self.image_subscription = self.create_subscription(
            Image, '/camera_1/image_raw_1', self.image_callback, 10)
        
        # YOLO检测结果订阅
        self.yolo_sub = self.create_subscription(
            DetectionArray, 'detections', self.detection_callback, qos_profile=self.detect_qos)
        
        # ==================== 发布器 ====================
        # 发布标注了投影点的图像
        self.image_publisher = self.create_publisher(Image, '/image_lidar1', 10)
        
        # 发布检测目标的3D位置信息
        self.detected_object_array_publisher = self.create_publisher(
            DetectedObjectArray, '/detected_object_position1', qos_profile=self.detect_qos)
        
        # 发布每个目标的独立点云
        self.object_pointcloud_publisher = self.create_publisher(
            PointCloud2, '/detected_object_pointcloud1', qos_profile=self.detect_qos)
        
        # ==================== 性能统计 ====================
        self._frame_count = 0  # 处理帧数计数
        self._last_time = time.time()  # 上次统计时间
    
    def load_yaml_file(self, yaml_file):
        """
        加载YAML配置文件
        
        参数:
            yaml_file: str - YAML文件路径
            
        返回:
            dict: 解析后的配置数据
        """
        with open(yaml_file, 'r') as file:
            return yaml.safe_load(file)
    
    def camera_info_callback(self, msg):
        """
        相机内参回调函数
        
        从CameraInfo消息中提取相机内参矩阵K
        K = [fx,  0, cx]
            [ 0, fy, cy]
            [ 0,  0,  1]
            
        参数:
            msg: CameraInfo - 相机信息消息
        """
        # 从扁平化的内参矩阵中提取参数
        # msg.k 是9个float64的列表，按行优先存储3x3矩阵
        self.fx = msg.k[0]  # K[0,0] - x方向焦距
        self.fy = msg.k[4]  # K[1,1] - y方向焦距
        self.cx = msg.k[2]  # K[0,2] - 主点x
        self.cy = msg.k[5]  # K[1,2] - 主点y
        
        # 更新图像尺寸
        self.image_width = msg.width
        self.image_height = msg.height
        
        # 当所有内参都获取后，初始化点云投影器
        if self.projector is None and all(v is not None for v in [self.fx, self.fy, self.cx, self.cy]):
            self.projector = PointCloudProjector(
                self.transformation_matrix,
                self.fx, self.fy, self.cx, self.cy,
                self.image_width, self.image_height
            )
            self.get_logger().info('PointCloudProjector initialized')
    
    def image_callback(self, msg):
        """
        图像回调函数
        
        将ROS图像消息转换为OpenCV格式并缓存
        
        参数:
            msg: Image - ROS图像消息
        """
        # 转换为BGR格式的OpenCV图像 (便于显示)
        self.current_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')
    
    def detection_callback(self, msg):
        """
        YOLO检测结果回调函数
        
        处理流程:
            1. 从DetectionArray提取原始检测框
            2. 送入多目标跟踪器获得稳定跟踪框和ID
            3. 缓存处理后的检测信息供点云回调使用
            
        参数:
            msg: DetectionArray - YOLO检测结果
        """
        # 简单的互斥: 如果正在处理则跳过本次
        if self._detection_lock:
            return
        
        self._detection_lock = True
        
        try:
            # ========== 步骤1: 提取原始检测框 ==========
            raw_detections = []      # 边界框列表 [[x1,y1,x2,y2], ...]
            detection_info = []      # 完整检测信息列表
            
            for detection in msg.detections:
                # 从中心点和尺寸计算边界框
                # msg中的bbox是center + size格式
                cx = detection.bbox.center.position.x
                cy = detection.bbox.center.position.y
                w = detection.bbox.size.x
                h = detection.bbox.size.y
                
                # 转换为 [x1, y1, x2, y2] 格式
                x1 = cx - w * 0.5
                y1 = cy - h * 0.5
                x2 = cx + w * 0.5
                y2 = cy + h * 0.5
                
                raw_detections.append([x1, y1, x2, y2])
                
                # 保存其他检测信息
                detection_info.append({
                    'class_id': detection.class_id,      # 类别ID
                    'class_name': detection.class_name,  # 类别名称 (如"cone")
                    'score': detection.score             # 置信度分数 [0,1]
                })
            
            # ========== 步骤2: 执行多目标跟踪 ==========
            # tracked_results: [(track_id, bbox), ...]
            tracked_results = self.fusion.update(raw_detections)
            
            # ========== 步骤3: 构建检测结果缓存 ==========
            self.detections_cache = []
            
            for track_id, bbox in tracked_results:
                # 通过IOU匹配找到对应的原始检测信息
                best_iou = 0.0
                best_info = None
                
                for i, raw_bbox in enumerate(raw_detections):
                    iou = MultiTargetTracker.iou(bbox, raw_bbox)
                    if iou > best_iou:
                        best_iou = iou
                        best_info = detection_info[i]
                
                # 添加到缓存
                self.detections_cache.append({
                    'bbox': bbox,                                    # 边界框 (x1,y1,x2,y2)
                    'track_id': track_id,                           # 跟踪ID
                    'class_id': best_info['class_id'] if best_info else -1,
                    'class_name': best_info['class_name'] if best_info else '',
                    'score': best_info['score'] if best_info else 0.0,
                    'cx': (bbox[0] + bbox[2]) * 0.5,                # 中心x
                    'cy': (bbox[1] + bbox[3]) * 0.5                 # 中心y
                })
                
        finally:
            self._detection_lock = False
    
    def pointcloud_callback(self, msg):
        """
        点云回调函数 - 主处理逻辑
        
        参数:
            msg: PointCloud2 - 激光雷达点云消息
        """
        # ========== 前置条件检查 ==========
        if self.current_image is None:
            # 还没有收到图像
            return
        
        if self.projector is None:
            # 相机内参还未就绪
            return
        
        # ========== 提取有效点云 ==========
        points = []
        # 遍历点云消息中的每个点
        # field_names指定我们需要的字段
        # skip_nans=True自动跳过NaN值
        for point in pc2.read_points(msg, field_names=("x", "y", "z"), skip_nans=True):
            x, y, z = point[0], point[1], point[2]
            
            # 深度范围过滤: 只保留在有效范围内的点
            # 注意: 这里用的是x坐标 (激光雷达的前向距离)
            if self.depth_range_min < x < self.depth_range_max:
                points.append([x, y, z])
        
        if not points:
            # 没有有效点云，仍发布原始图像
            img_msg = self.bridge.cv2_to_imgmsg(self.current_image, encoding='bgr8')
            img_msg.header = msg.header
            self.image_publisher.publish(img_msg)
            return
        
        # ========== 如果没有检测框 ==========
        if not self.detections_cache:
            # 无检测时仍可以投影点云到图像上显示
            transformed_points, u_arr, v_arr = self.projector.project_batch(points)
            
            image_with_points = self.current_image.copy()
            for u, v in zip(u_arr, v_arr):
                cv2.circle(image_with_points, (int(u), int(v)), 2, (0, 0, 255), -1)
            
            img_msg = self.bridge.cv2_to_imgmsg(image_with_points, encoding='bgr8')
            img_msg.header = msg.header
            self.image_publisher.publish(img_msg)
            return
        
        # ========== 批量投影点云 ==========
        transformed_points, u_arr, v_arr = self.projector.project_batch(points)
        
        # ========== 点云与检测框匹配 ==========
        image_with_points = self.current_image.copy()
        num_dets = len(self.detections_cache)
        
        # 初始化每个检测框的点云收集器
        # bbox_points[i] 收集落在第i个检测框内的所有点云
        bbox_points = [
            {
                'x': [],       # 相机坐标系下的X坐标列表
                'y': [],       # 相机坐标系下的Y坐标列表
                'z': [],       # 相机坐标系下的Z坐标列表 (深度)
                'points': []   # 原始点云坐标 [x, y, z] (激光雷达坐标系)
            }
            for _ in range(num_dets)
        ]
        
        # 遍历所有有效投影点
        for i, (tp, u, v) in enumerate(zip(transformed_points, u_arr, v_arr)):
            tx, ty, tz = tp[0], tp[1], tp[2]
            
            # 在图像上绘制投影点 (红色圆点)
            cv2.circle(image_with_points, (int(u), int(v)), 2, (0, 0, 255), -1)
            
            # 检查该点落在哪个检测框内
            for di, det in enumerate(self.detections_cache):
                x1, y1, x2, y2 = det['bbox']
                
                # 点在框内判断
                if x1 <= u <= x2 and y1 <= v <= y2:
                    # 将点添加到对应框的收集器中
                    bbox_points[di]['x'].append(tx)
                    bbox_points[di]['y'].append(ty)
                    bbox_points[di]['z'].append(tz)
                    bbox_points[di]['points'].append(points[i])
                    break  # 一个点只属于一个框 (最近原则)
        
        # ========== 发布带投影点的图像 ==========
        img_msg = self.bridge.cv2_to_imgmsg(image_with_points, encoding='bgr8')
        img_msg.header = msg.header
        self.image_publisher.publish(img_msg)
        
        # ========== 发布检测目标信息 ==========
        self.publish_detected_objects(bbox_points, msg.header)
        self.publish_object_pointclouds(bbox_points, msg.header)
        
        # ========== 性能统计 (每30帧输出一次) ==========
        self._frame_count += 1
        if self._frame_count % 30 == 0:
            now = time.time()
            fps = 30.0 / (now - self._last_time)
            self.get_logger().debug(f'Fusion node FPS: {fps:.1f}')
            self._last_time = now
    
    def publish_detected_objects(self, bbox_points, header):
        """
        发布检测到的目标3D位置
        
        参数:
            bbox_points: list - 每个检测框收集的点云数据
            header: std_msgs/Header - 用于发布消息的时间戳和坐标系
        """
        # 创建检测目标数组消息
        detected_objects_msg = DetectedObjectArray()
        detected_objects_msg.header = header
        detected_objects_msg.header.frame_id = "camera1_link"  # 坐标系标识# 坐标系标识# 坐标系标识# 坐标系标识# 坐标系标识# 坐标系标识
        
        for i, values in enumerate(bbox_points):
            # 跳过没有点云的目标
            if not values['points']:
                continue
            
            if i >= len(self.detections_cache):
                continue
            
            obj_msg = DetectedObject()
            det = self.detections_cache[i]
            
            # ========== 获取跟踪器质量信息 ==========
            # 从跟踪器获取协方差迹和IOU分数
            track = self.fusion.tracks[i] if i < len(self.fusion.tracks) else None
            
            # covariance_trace: 卡尔曼协方差的迹，表示状态估计的不确定性
            # 值越小表示估计越可靠
            obj_msg.covariance_trace = track.covariance_trace if track else 0.0
            
            # iou_score: 最近一次匹配的IOU分数
            obj_msg.iou_score = track.iou_score if track else 0.0
            
            # ========== 检测基本信息 ==========
            obj_msg.track_id = det['track_id']          # 跟踪ID
            obj_msg.score = det['score']                # 检测置信度
            obj_msg.class_id = det['class_id']          # 类别ID
            obj_msg.class_name = det['class_name']      # 类别名称
            
            # ========== 计算目标3D位置 ==========
            # 使用中位数而不是均值，因为中位数对离群点更鲁棒
            # 点云中可能存在误匹配的噪声点，中位数能有效抑制
            raw_lidar_x = float(np.median(values['x']))
            raw_lidar_y = float(np.median(values['y']))
            raw_lidar_z = float(np.median(values['z']))
            
            # ========== 激光雷达坐标卡尔曼平滑 ==========
            # 对x,y坐标进行卡尔曼滤波，减少帧间抖动
            self.kf_lidar.predict()
            smooth_x, smooth_y = self.kf_lidar.update([raw_lidar_x, raw_lidar_y])
            
            # ========== 设置目标位姿 ==========
            obj_msg.pose.position.x = smooth_x     # 平滑后的x
            obj_msg.pose.position.y = smooth_y     # 平滑后的y
            obj_msg.pose.position.z = raw_lidar_z  # z直接使用原始值
            
            # 方向设为默认值 (无旋转)
            obj_msg.pose.orientation.w = 1.0
            obj_msg.pose.orientation.x = 0.0
            obj_msg.pose.orientation.y = 0.0
            obj_msg.pose.orientation.z = 0.0
            
            # ========== 设置边界框信息 ==========
            x1, y1, x2, y2 = det['bbox']
            obj_msg.bbox.center.position.x = (x1 + x2) * 0.5  # 中心x
            obj_msg.bbox.center.position.y = (y1 + y2) * 0.5  # 中心y
            obj_msg.bbox.center.position.z = 0.0
            obj_msg.bbox.size.x = x2 - x1  # 宽度
            obj_msg.bbox.size.y = y2 - y1  # 高度
            obj_msg.bbox.size.z = 0.0
            
            detected_objects_msg.objects.append(obj_msg)
        
        # 发布消息
        self.detected_object_array_publisher.publish(detected_objects_msg)
    
    def publish_object_pointclouds(self, bbox_points, header):
        """
        发布每个目标的独立点云
        
        参数:
            bbox_points: list - 每个检测框收集的点云数据
            header: std_msgs/Header - 用于发布消息的时间戳和坐标系
        """
        for values in bbox_points:
            if not values['points']:
                continue
            
            points = values['points']
            
            # 定义点云字段: x, y, z (float32)
            fields = [
                PointField(name='x', offset=0, datatype=PointField.FLOAT32, count=1),
                PointField(name='y', offset=4, datatype=PointField.FLOAT32, count=1),
                PointField(name='z', offset=8, datatype=PointField.FLOAT32, count=1),
            ]
            
            # 打包点云数据为二进制
            # 每个点3个float32，共12字节
            point_data = b''
            for p in points:
                # struct.pack('fff', ...) 将3个float打包为12字节
                point_data += struct.pack('fff', p[0], p[1], p[2])
            
            # 构建PointCloud2消息
            pc_msg = PointCloud2(
                header=header,
                height=1,                          # 无序点云
                width=len(points),                 # 点数量
                is_dense=False,                    # 可能包含无效点
                is_bigendian=False,                # 小端字节序
                fields=fields,                     # 字段定义
                point_step=12,                     # 每个点12字节
                row_step=12 * len(points),         # 每行字节数
                data=point_data                    # 二进制数据
            )
            
            # 发布该目标的点云
            self.object_pointcloud_publisher.publish(pc_msg)


def main(args=None):
    """
    主函数 - ROS2节点入口
    
    参数:
        args: 命令行参数
    """
    # 初始化ROS2 Python客户端库
    rclpy.init(args=args)
    
    # 创建融合节点实例
    node = LidarCameraFusionNode()
    
    try:
        # 进入ROS2事件循环，阻塞直到节点被关闭
        # spin会持续处理回调函数
        rclpy.spin(node)
    except KeyboardInterrupt:
        # 捕获Ctrl+C，优雅退出
        pass
    finally:
        # 清理资源
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
