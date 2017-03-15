#!/usr/bin/env python
# coding: utf-8
import sys
import rospy
import copy
import geometry_msgs.msg

from std_msgs.msg import Float32MultiArray

from moveit_commander import MoveGroupCommander
from moveit_commander import roscpp_initialize, roscpp_shutdown

from math import sin, copysign

next_pose = geometry_msgs.msg.Pose()

right_shoulder_to_wrist = MoveGroupCommander("right_shoulder_to_wrist")

def right_shoulder_to_wrist_callback(data):	
	next_pose.position.x = data.data[0]
	next_pose.position.y = data.data[1]
	next_pose.position.z = data.data[2]

def mimic_positions():
	while not rospy.is_shutdown():
		right_shoulder_to_wrist.set_pose_target(next_pose)
		plan_next = right_shoulder_to_wrist.plan()
		rospy.sleep(5)
		right_shoulder_to_wrist.execute(plan_next)
		rospy.sleep(5)
		rospy.spin()
	

if __name__ == '__main__':
	
	rospy.init_node('mimic', anonymous=True)
	
	# Set the start pose
	start_pose = geometry_msgs.msg.Pose() # Find how to set to custom pose - right_hand_kinect_start

	# Values for right_hand_kinect_start
    	start_pose.position.x = 0.0464712799415
    	start_pose.position.y = 0.0737200235193
    	start_pose.position.z = 1.3721234889
    	start_pose.orientation.x = 0.212880825044
    	start_pose.orientation.y = 0.806655581926
    	start_pose.orientation.z = 0.440611611646
    	start_pose.orientation.w = 0.331436169057	

	right_shoulder_to_wrist.set_pose_target(start_pose)
	plan_start = right_shoulder_to_wrist.plan() #Facing problems in planning with this mouve group
	rospy.sleep(5)
	right_shoulder_to_wrist.execute(plan_start)
	rospy.sleep(5)
	
	# Set the orientation for next pose	
	next_pose.orientation.x = 0.5
    	next_pose.orientation.y = 0.5
    	next_pose.orientation.z = 0.5
    	next_pose.orientation.w = 0.5
	
	# Subscriber
	rospy.Subscriber("/right_shoulder_to_wrist_transform", Float32MultiArray, right_shoulder_to_wrist_callback)
	
	# Start mimic
	try:
		mimic_positions()
	except rospy.ROSInterruptException:
		pass


