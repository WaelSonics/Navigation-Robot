#command:
#ros2 topic pub /demo/cmd_vel geometry_msgs/msg/Twist '{linear: {x: 0.2, y: 0.0, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 0.2}}'


# Import necessary ROS 2 modules
import rclpy
from geometry_msgs.msg import Twist
import time

def main(args=None):
    # Initialize the ROS 2 node
    rclpy.init(args=args)
    node = rclpy.create_node("robot_control_node")

    # Create a publisher for Twist messages
    pub = node.create_publisher(Twist, "/demo/cmd_vel", 10)

    # Create a Twist message
    twist = Twist()

    # Set linear and angular velocities
    twist.linear.x = 0.1  # Adjust this value as needed for your desired linear velocity
    twist.angular.z = 0.0  # Adjust this value for angular velocity

    try:
        while rclpy.ok():
            # Publish the Twist message
            pub.publish(twist)
            time.sleep(0.1)  # Adjust the sleep time to control the publishing rate
    except KeyboardInterrupt:
        pass

    # Clean up and shutdown the ROS 2 node
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

