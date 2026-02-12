# Arduino uno Q dependencies
from ros_led.app_utils import *

# ROS Dependencies
import rclpy
from rclpy.node import Node

from std_msgs.msg import Bool

class MyRos2Node(Node):
    def __init__(self):
        super().__init__('ros_led_node')
        self.get_logger().info('Node ros_led_node initialized')

        self.subscription = self.create_subscription(
            Bool,
            'led_status',
            self.listener_callback,
            10
        )
        self.subscription

    def listener_callback(self, msg):
        self.get_logger().info(f'Message Received: {msg.data}')
        Bridge.call("set_led_state", msg.data)

def main(args=None):
    rclpy.init(args=args)
    node = MyRos2Node()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()