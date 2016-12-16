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

def callback(data):	
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

	next_pose.orientation.x = 0.5
    	next_pose.orientation.y = 0.5
    	next_pose.orientation.z = 0.5
    	next_pose.orientation.w = 0.5

	rospy.Subscriber("/right_shoulder_to_wrist_transform", Float32MultiArray, right_shoulder_to_wrist_callback)

	try:
		mimic_positions()
	except rospy.ROSInterruptException:
		pass


