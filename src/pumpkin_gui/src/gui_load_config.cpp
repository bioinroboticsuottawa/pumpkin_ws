//
// Created by rafaelpaiva on 18/09/15.
//

//ROS inlcudes
#include <ros/ros.h>

//GTKmm includes
#include <gtkmm/application.h>
#include <pumpkin_messages/Files.h>

//Class instatiation
#include "RobotGUI.h"
#include "LoadConfig.h"

int main (int argc, char *argv[]) {

	ros::init(argc, argv, "gui_load_config");
	ros::NodeHandle nh;

	auto main_app = Gtk::Application::create(argc, argv, "robot_interface.pumpkin.ros");
	LoadConfig load;

	int status = 0;

	if (!ros::param::has("/pumpkin/config/ssc")) {

		auto load_app = Gtk::Application::create(argc, argv, "robot_interface.pumpkin.ros");

		ros::ServiceClient config_client = nh.serviceClient<pumpkin_messages::Files>("file_lister");
		pumpkin_messages::Files msg;
		config_client.waitForExistence();
		msg.request.type = (uint8_t)1;
		if (!config_client.call(msg)) {
			ROS_ERROR("Cannot get configuration files list");
			return -1;
		}

		std::string path(msg.response.base_path), filename;
		path += "/";
		path += msg.response.files[0].folder;

		load.link_file_string(&filename);
		load.fill_combobox(msg.response.files[0].filenames);
		ROS_INFO("Select a load configuration file on the dialog.");
		status = load_app->run(load);

		if (status != 0)
			return status;

		load_app.reset();

		ROS_INFO("Select configuration %s", filename.c_str());

		if (!filename.empty()) {
			ros::param::set("/pumpkin/_config_file", path + "/" + filename);

			/*ros::Rate r(1000);
			while (!ros::param::has("/pumpkin/config"))
				r.sleep();*/
		} else {
			ROS_FATAL("Error on opening configuration file.");
		}
	}

	return status;

	/*
	ros::ServiceClient client = nh.serviceClient<pumpkin_messages::SSCMoveCommand>("move_ssc");

	if (!client.isValid()) {
		ROS_ERROR("Service is unavailable now. Check the \"setup_ssc\" node.");
		return -2;
	}

	RobotGUI gui;

	gui.set_service(client);

	status = main_app->run(gui);

	return status;*/
}