Dr Robot Hawk Robot (A.K.A Pumpkin) catkin workspace
====================================================

This repository contains catkin source files to Dr Robot Hawk Robot (A.K.A Pumpkin).

There are the following packages:
* pumpkin: All applications to record and playback movements with pumpkin.
* pumpkin_moveit: Moveit package for pumpkin.
* pumpkin_description: Description package for pumpkin.
* pumpkin_messages: Contains all messages and actions used in our solution.
* pumpkin_interface: Main interface code for Arduino, SSC-32 and some remote file operations.
* pumpkin_qt: GUI for pumpkin.
* ~~analog_array: Which implements arduino analog servo reads messages. (moved to pumpkin_messages)~~

>Currently initial script (`pumpkin.sh` loaded when `/etc/init.d/init_pumpkin` run) load only `pumkin_kinect.launch`.

>On the robot `pumpkin.launch` rises an error loading on start-up probably because SSC port (`ttyUSB0`) is not ready yet.

To build:

    $ catkin_make
    
Maybe you will need to run `catkin_make` twice because analog_array and pumpking_messages packages may have self references.

Basically you can run the main software on the robot using:

    $ roslaunch pumpkin pumpkin.launch

And the GUI node:

    $ rosrun pumpkin_qt pumpkin_qt
    
Some configuration files and initialization scripts can be found in the root directory:

* gui.sh: GUI script that configures ROS_MASTER and run pumpkin_qt.
* pumpkin.sh: Configuration and launch of pumpkin.launch
* init_pumpkin: Script that must be copied to /etc/init.rc in order to have a standalone computer booting the pumpkin master code.

To configure your IDE there are some instructions [here](http://wiki.ros.org/IDEs). After some tests we now use [Clion](https://www.jetbrains.com/clion/).

Documentation and manual can be found under `doc/` and `manual/` folders, respectively.

Udev Rules
----------

We defined udev rules to get easy-to-remember and constant symlinks to tty ports.
Tutorial on how to define udev rules for arduino boards [here](http://vncprado.github.io/udev-rules-for-ttyusb/)
Create file:

    sudo nano /etc/udev/rules.d/99-usb-serial.rules
    
and fill with the rules. Example:

    SUBSYSTEM=="tty", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="0483", ATTRS{serial}=="2337390", SYMLINK+="ttyUSB.ada"
    SUBSYSTEM=="tty", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="0483", ATTRS{serial}=="2337330", SYMLINK+="ttyUSB.dextrus"
    
TinyXML problem:

Modifying /opr/ros/indigo/include/pluginlib/class_loader.h, including "#define TIXML_USE_STL" just before #include "tinyxml.h" solves the problem.
[source](http://answers.ros.org/question/206042/vrep_ros_bridge-installation-problem/)
