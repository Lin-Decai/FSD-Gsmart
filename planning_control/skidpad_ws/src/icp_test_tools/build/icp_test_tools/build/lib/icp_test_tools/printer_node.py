import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64MultiArray

class TransformPrinter(Node):
    def __init__(self):
        super().__init__('transform_printer')
        self.sub = self.create_subscription(
            Float64MultiArray,
            '/transform_matrix',
            self.callback,
            10)
        self.get_logger().info('Transform printer ready')

    def callback(self, msg):
        if len(msg.data) == 16:
            mat = [msg.data[i:i+4] for i in range(0, 16, 4)]
            self.get_logger().info('Received transformation matrix:')
            for row in mat:
                self.get_logger().info(f'  [{row[0]:8.4f}, {row[1]:8.4f}, {row[2]:8.4f}, {row[3]:8.4f}]')
        else:
            self.get_logger().warn(f'Received data with length {len(msg.data)}, expected 16')

def main(args=None):
    rclpy.init(args=args)
    node = TransformPrinter()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()