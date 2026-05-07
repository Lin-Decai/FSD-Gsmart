std_msgs/Header header
int32 label          # 目标类别（如0=红色锥桶，1=绿色锥桶）
float32 confidence   # 置信度（0~1）
float32 x            # 3D坐标x（雷达坐标系）
float32 y            # 3D坐标y（雷达坐标系）
float32 z            # 3D坐标z（雷达坐标系）
float32 width        # 目标宽度（x方向）
float32 length       # 目标长度（y方向）
float32 height       # 目标高度（z方向）
string source        # 来源："fused"/"radar_only"/"camera_only"

