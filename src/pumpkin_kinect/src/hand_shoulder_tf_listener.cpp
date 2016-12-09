#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <std_msgs/Float32MultiArray.h>

std_msgs::Float32MultiArray positionTransform; // (x,y,z)

int main(int argc, char** argv) {

	ros::init(argc, argv, "hand_shoulder_tf_listener");
   	ros::NodeHandle nh;

	positionTransform.data.resize(3);
    	//double x, y, z;
    
    	//Publisher
    	ros::Publisher position_transform_pub = nh.advertise<std_msgs::Float32MultiArray>("/position_transform", 32); // Publish the position transform shoulder - hand

    	tf::TransformListener listener;

	// Get the transform: hand's position from the shoulder
    	//ros::Rate rate(10.0);
    	while (nh.ok()) {
        	tf::StampedTransform transform;
        	try {
            		listener.lookupTransform("right_shoulder_1", "right_hand_1", ros::Time(0), transform);
        	}
        	catch (tf::TransformException &ex) {
            		ROS_ERROR("%s", ex.what());
            		//ros::Duration(1.0).sleep();
            		continue;
        	}

        positionTransform.data[0] = transform.getOrigin().x();
        positionTransform.data[1] = transform.getOrigin().y();
        positionTransform.data[2] = transform.getOrigin().z();

	position_transform_pub.publish(positionTransform);

	//ROS_INFO("Ok!");
    	}
	ros::shutdown();
	
	return 0;
}
