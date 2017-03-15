#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <geometry_msgs/Pose.h>
#include <moveit/move_group_interface/move_group.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

moveit::planning_interface::MoveGroup right_shoulder_to_wrist("right_shoulder_to_wrist");
moveit::planning_interface::MoveGroup::Plan plan_next;
geometry_msgs::Pose next_pose;

// Callback function for the topic '/right_shoulder_to_wrist_transform'
void right_shoulder_to_wrist_transformCb(const std_msgs::Float32MultiArray& transform_msg) {
	next_pose.position.x = transform_msg.data[0];
	next_pose.position.y = transform_msg.data[1];
	next_pose.position.z = transform_msg.data[2];
}

void mimic_positions() {
	right_shoulder_to_wrist.setPoseTarget(next_pose);
	right_shoulder_to_wrist.plan(plan_next);
	sleep(5.0);
	right_shoulder_to_wrist.execute(plan_next);
	sleep(5.0);
}

int main (int argc, char **argv) {
    	ros::init(argc, argv, "mimic");
    	ros::NodeHandle nh;
	
	// Set the start pose
	geometry_msgs::Pose start_pose;

	right_shoulder_to_wrist.setPoseTarget(start_pose);
	moveit::planning_interface::MoveGroup::Plan plan_start;
	right_shoulder_to_wrist.plan(plan_start);
	sleep(5.0);
	right_shoulder_to_wrist.execute(plan_start);
	sleep(5.0);	

	// Set the orientation for next pose
	next_pose.orientation.x = 0.5;
    	next_pose.orientation.y = 0.5;
    	next_pose.orientation.z = 0.5;
    	next_pose.orientation.w = 0.5;

	//Subscriber
    	ros::Subscriber right_shoulder_to_wrist_transform_sub = nh.subscribe("/right_shoulder_to_wrist_transform", 32, right_shoulder_to_wrist_transformCb);


	ROS_INFO("AQUI1");
	// Start mimic
	while (ros::ok()) {
		mimic_positions();
		ros::spinOnce();
	}
	
}
