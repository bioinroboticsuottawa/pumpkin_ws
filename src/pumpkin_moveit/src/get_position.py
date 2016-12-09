#!/usr/bin/env python
# coding: utf-8
import sys
import rospy
import copy
import geometry_msgs.msg

from moveit_commander import MoveGroupCommander
from moveit_commander import roscpp_initialize, roscpp_shutdown

from math import sin, copysign

if __name__ == '__main__':
	print "--- Straight line gesture ---"
	rospy.init_node('straight_line', anonymous=True)
	right_arm = MoveGroupCommander("right_arm")

	start_pose = geometry_msgs.msg.Pose()
        start_pose.position.x = -0.0206330384032
    	start_pose.position.y = 0.077582282778
    	start_pose.position.z = 1.39283677496
    	start_pose.orientation.x = 0.5
    	start_pose.orientation.y = 0.5
    	start_pose.orientation.z = 0.5
    	start_pose.orientation.w = 0.5

	right_arm.set_pose_target(start_pose)	
	plan_start = right_arm.plan()
   	print "Plan start"
    	rospy.sleep(5)
    	right_arm.execute(plan_start)
    	print "Execute start"
    	rospy.sleep(5)

	end_pose = geometry_msgs.msg.Pose()
        end_pose.position.x = -0.0434279649929
    	end_pose.position.y = -0.0562017053887
    	end_pose.position.z = 1.48763433664
    	end_pose.orientation.x = 0.5
    	end_pose.orientation.y = 0.5
    	end_pose.orientation.z = 0.5
    	end_pose.orientation.w = 0.5

	right_arm.set_pose_target(end_pose)	
	plan_end = right_arm.plan()
   	print "Plan end"
    	rospy.sleep(5)
    	right_arm.execute(plan_end)
    	print "Execute end"
    	rospy.sleep(5)

	#rospy.sleep(1)	
	
	current_pose = geometry_msgs.msg.Pose()
	current_pose = right_arm.get_current_pose()
	
	print(current_pose)

	print "---------------------------------------End"
