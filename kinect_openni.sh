#!/bin/bash
source devel/setup.bash

roslaunch openni_launch openni.launch &
sleep 10
rosparam set /openni_tracker/camera_frame_id camera_link &
echo "param ok"
sleep 10
echo "merda"
rosrun openni_tracker openni_tracker __name:=my_node1&
