import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/x/Documents/at20260415workspace/workspace/camera/install/camera_node'
