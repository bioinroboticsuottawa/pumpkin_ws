#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <std_msgs/Float32MultiArray.h>
#include <visualization_msgs/Marker.h>

std_msgs::Float32MultiArray positionTransform; // (x,y,z)
visualization_msgs::Marker marker_right_shoulder;
visualization_msgs::Marker marker_right_hand;
visualization_msgs::Marker marker_transform;

int main(int argc, char** argv) {

	ros::init(argc, argv, "hand_shoulder_tf_listener");
   	ros::NodeHandle nh;
	ros::Rate r(10.0);

	positionTransform.data.resize(3); // (x,y,z)
    
    	//Publishers
    	ros::Publisher position_transform_pub = nh.advertise<std_msgs::Float32MultiArray>("/right_shoulder_to_wrist_transform", 32); // Publish the position transform from shoulder to hand
	ros::Publisher marker_right_shoulder_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker_right_shoulder", 1);
	ros::Publisher marker_right_hand_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker_right_hand", 1);
	ros::Publisher marker_transform_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker_transform", 1);

	// Set the shape type to be a sphere
	uint32_t shape = visualization_msgs::Marker::SPHERE;
	

// Marker for right shoulder - Green
	// Set the frame ID and timestamp for shoulder's marker. 
	marker_right_shoulder.header.frame_id = "right_shoulder_1";
	marker_right_shoulder.header.stamp = ros::Time::now();
	// Set the namespace and id for this marker.
	marker_right_shoulder.ns = "hand_shoulder_tf_listener";
	marker_right_shoulder.id = 0;
	// Set the marker type. It's a SPHERE
	marker_right_shoulder.type = shape;
	// Set the marker action.  Options are ADD and DELETE	
	marker_right_shoulder.action = visualization_msgs::Marker::ADD;	
	// Set the scale of the marker -- 1x1x1 here means 1m on a side
	marker_right_shoulder.scale.x = 0.1;
	marker_right_shoulder.scale.y = 0.1;
	marker_right_shoulder.scale.z = 0.1;
	// Set the color -- be sure to set alpha to something non-zero!
	marker_right_shoulder.color.r = 0.0f;
	marker_right_shoulder.color.g = 1.0f;
	marker_right_shoulder.color.b = 0.0f;
	marker_right_shoulder.color.a = 1.0;
	// Set the pose of the marker - 
	marker_right_shoulder.pose.position.x = 0;
	marker_right_shoulder.pose.position.y = 0;
	marker_right_shoulder.pose.position.z = 0;
	marker_right_shoulder.pose.orientation.x = 0.0;
	marker_right_shoulder.pose.orientation.y = 0.0;
	marker_right_shoulder.pose.orientation.z = 0.0;
	marker_right_shoulder.pose.orientation.w = 1.0;
	marker_right_shoulder.lifetime = ros::Duration();

// Marker for right hand - Red
	marker_right_hand.header.frame_id = "right_hand_1";
	marker_right_hand.header.stamp = ros::Time::now();
	marker_right_hand.ns = "hand_shoulder_tf_listener";
	marker_right_hand.id = 1;
	marker_right_hand.type = shape;
	marker_right_hand.action = visualization_msgs::Marker::ADD;
	marker_right_hand.scale = marker_right_shoulder.scale;
	marker_right_hand.color.r = 1.0f;
	marker_right_hand.color.g = 0.0f;
	marker_right_hand.color.b = 0.0f;
	marker_right_hand.color.a = 1.0;
	marker_right_hand.pose = marker_right_shoulder.pose;
	marker_right_hand.lifetime = ros::Duration();

// Marker for transform shoulder to hand - Blue
	marker_transform.header.frame_id = "right_shoulder_1";
	marker_transform.header.stamp = ros::Time::now();
	marker_transform.ns = "hand_shoulder_tf_listener";
	marker_transform.id = 2;
	marker_transform.type = shape;
	marker_transform.action = visualization_msgs::Marker::ADD;
	marker_transform.scale = marker_right_shoulder.scale;
	marker_transform.color.r = 0.0f;
	marker_transform.color.g = 0.0f;
	marker_transform.color.b = 1.0f;
	marker_transform.color.a = 1.0;
	marker_transform.pose.orientation.x = 0.0;
	marker_transform.pose.orientation.y = 0.0;
	marker_transform.pose.orientation.z = 0.0;
	marker_transform.pose.orientation.w = 1.0;
	marker_transform.lifetime = ros::Duration();

    	tf::TransformListener listener;

	// Get the transform: hand's position from the shoulder
    	//ros::Rate rate(10.0);
    	while (nh.ok()) {
        	tf::StampedTransform transform;
        	try {
            		listener.lookupTransform("right_hand_1", "right_shoulder_1", ros::Time(0), transform);
        	}
        	catch (tf::TransformException &ex) {
            		ROS_ERROR("%s", ex.what());
            		//ros::Duration(1.0).sleep();
            		continue;
        	}

        positionTransform.data[0] = transform.getOrigin().x();
        positionTransform.data[1] = transform.getOrigin().y();
        positionTransform.data[2] = transform.getOrigin().z();

	marker_transform.pose.position.x = transform.getOrigin().x();
	marker_transform.pose.position.y = transform.getOrigin().y();
	marker_transform.pose.position.z = transform.getOrigin().z();

	ROS_INFO("x = %f",marker_transform.pose.position.x);
	ROS_INFO("y = %f",marker_transform.pose.position.y);	
	ROS_INFO("z = %f",marker_transform.pose.position.z);	
	
	position_transform_pub.publish(positionTransform);

	marker_right_shoulder_pub.publish(marker_right_shoulder);
	marker_right_hand_pub.publish(marker_right_hand);
	marker_transform_pub.publish(marker_transform);
	
	r.sleep();
	
	//ROS_INFO("Ok!");
    	}
	ros::shutdown();
	
	return 0;
}
