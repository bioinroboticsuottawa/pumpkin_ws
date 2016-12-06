#!/bin/bash
MASTER=$HOSTNAME
cd ~/workspace/pumpkin_ws/
source devel/setup.bash
export ROS_MASTER_URI=http://$MASTER:11311

echo pumpkin kinect init
roslaunch pumpkin pumpkin_kinect.launch &
sleep 10s
echo Head init...
rosrun pumpkin head_init.py &
sleep 10s
echo pumpkin init
#roslaunch pumpkin pumpkin.launch &
