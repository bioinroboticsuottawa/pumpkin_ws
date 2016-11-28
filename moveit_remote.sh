#!/bin/bash
MASTER=pumpkinpi
source devel/setup.bash
export ROS_MASTER_URI=http://$MASTER:11311
roslaunch pumpkin_moveit demo.launch
