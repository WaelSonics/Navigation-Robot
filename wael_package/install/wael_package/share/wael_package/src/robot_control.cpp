#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

class RobotControlNode : public rclcpp::Node {
public:
    RobotControlNode() : Node("robot_controller") {
      publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

      // Create and send velocity commands
      auto msg = std::make_shared<geometry_msgs::msg::Twist>();
      msg->linear.x = 0.2;  // Adjust linear velocity (e.g., 0.2 m/s)
      msg->angular.z = 0.0; // Adjust angular velocity (e.g., 0.0 rad/s)

      RCLCPP_INFO(this->get_logger(), "Moving forward...");
      for (int i = 0; i < 100; ++i) { // Publish for 5 seconds
          publisher_->publish(*msg);
          rclcpp::spin_some(this->get_node_base_interface());
          usleep(100000); // Sleep for 0.1 seconds
      }

      // Stop the robot
      msg->linear.x = 0.0;
      RCLCPP_INFO(this->get_logger(), "Moving backward...");
      for (int i = 0; i < 120; ++i) { // Publish for 6 seconds in reverse
          publisher_->publish(*msg);
          rclcpp::spin_some(this->get_node_base_interface());
          usleep(100000); // Sleep for 0.1 seconds
      }
     }

private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<RobotControlNode>());
    rclcpp::shutdown();
    return 0;
}

