#!/usr/bin/env python
# coding: utf-8
import sys
import rospy
import copy
import geometry_msgs.msg
import moveit_msgs


from std_msgs.msg import Float32MultiArray

from moveit_commander import MoveGroupCommander
from moveit_commander import roscpp_initialize, roscpp_shutdown


from math import sin, copysign

if __name__ == '__main__':
	rospy.init_node('mimic', anonymous=True)

	right_shoulder_to_wrist = MoveGroupCommander("right_arm")

	start_state = right_shoulder_to_wrist.getCurrentState();
	
	print(start_state)

	print "---------------------------------------End"
