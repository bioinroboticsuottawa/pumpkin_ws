#include <ros/ros.h>
#include <tf/transform_listener.h>

int main(int argc, char** argv) {
	
	ros::init(argc, argv, "test_positions");
   	ros::NodeHandle nh;

	double right_shoulder_openni_position[3];
	double right_hand_openni_position[3];	
	double shoulder_hand_transform_position[3];
	double check[3];

	tf::TransformListener listener;

	while (nh.ok()) {
        	tf::StampedTransform right_shoulder_openni_transform;
		tf::StampedTransform right_hand_openni_transform;	
		tf::StampedTransform shoulder_hand_transform;	
        	try {
            		listener.lookupTransform("openni_depth_frame", "right_shoulder_1", ros::Time(0), right_shoulder_openni_transform);
        	}
        	catch (tf::TransformException &ex) {
            		ROS_ERROR("%s", ex.what());
            		//ros::Duration(1.0).sleep();
            		continue;
        	}
		try {
            		listener.lookupTransform("openni_depth_frame", "right_hand_1", ros::Time(0), right_hand_openni_transform);
        	}
        	catch (tf::TransformException &ex) {
            		ROS_ERROR("%s", ex.what());
            		//ros::Duration(1.0).sleep();
            		continue;
        	}
		try {
            		listener.lookupTransform("right_hand_1", "right_shoulder_1", ros::Time(0), shoulder_hand_transform);
        	}
        	catch (tf::TransformException &ex) {
            		ROS_ERROR("%s", ex.what());
            		//ros::Duration(1.0).sleep();
            		continue;
        	}
	right_shoulder_openni_position[0] = right_shoulder_openni_transform.getOrigin().x();
        right_shoulder_openni_position[1] = right_shoulder_openni_transform.getOrigin().y();
        right_shoulder_openni_position[2] = right_shoulder_openni_transform.getOrigin().z();

	right_hand_openni_position[0] = right_hand_openni_transform.getOrigin().x();
        right_hand_openni_position[1] = right_hand_openni_transform.getOrigin().y();
        right_hand_openni_position[2] = right_hand_openni_transform.getOrigin().z();

	shoulder_hand_transform_position[0] = shoulder_hand_transform.getOrigin().x();
        shoulder_hand_transform_position[1] = shoulder_hand_transform.getOrigin().y();
        shoulder_hand_transform_position[2] = shoulder_hand_transform.getOrigin().z();

	check[0] = right_hand_openni_position[0] - right_shoulder_openni_position[0];
	check[1] = right_hand_openni_position[1] - right_shoulder_openni_position[1];
	check[2] = right_hand_openni_position[2] - right_shoulder_openni_position[2];

	
	ROS_INFO("x = %f\t\t%f\t\t%f\t\t%f\t\t", right_shoulder_openni_position[0], right_hand_openni_position[0], shoulder_hand_transform_position[0], check[0]);
	ROS_INFO("y = %f\t\t%f\t\t%f\t\t%f\t\t", right_shoulder_openni_position[1], right_hand_openni_position[1], shoulder_hand_transform_position[1], check[1]);
	ROS_INFO("z = %f\t\t%f\t\t%f\t\t%f\t\t", right_shoulder_openni_position[2], right_hand_openni_position[2], shoulder_hand_transform_position[2], check[2]);
	
	ros::Duration(1).sleep();
	}
	ros::shutdown();
	
	return 0;
}
