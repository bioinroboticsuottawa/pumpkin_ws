#include "ros/ros.h"
#include "yaml-cpp/yaml.h"

/*!
 * \brief This node loads the calibration config file for the Pumpkin into the ROS Parameter server.
 *
 * This node waits for the existence of a param containing the configuration file, it loads the file, and saves it into
 * the ROS Parameter Server. It reorganizes the configuration tree structure to best fit the others nodes.
 */
int main(int argc, char *argv[]) {
	//Init ROS Node
	ros::init(argc, argv, "load_config");
	ros::Time::init();

	//Seek file and load it
	std::string config_file;
	ros::Rate loop(1000);

	while (!ros::param::has("/pumpkin/_config_file"))
		loop.sleep();

	ros::param::get("/pumpkin/_config_file", config_file);

	YAML::Node config_load;
	try {
		config_load = YAML::LoadFile(config_file);
	} catch (YAML::BadFile &ex) {
		ROS_FATAL("Error loading file: %s", ex.what());
	}
	ROS_INFO("Opening config file: %s", config_file.c_str());
	XmlRpc::XmlRpcValue config_save;
	//Iterates for group part
	for (YAML::const_iterator it_cluster = config_load.begin(); it_cluster != config_load.end(); ++it_cluster)
	{
		const std::string &cluster = it_cluster->first.as<std::string>();
		if (cluster == "ros_rate") {
			config_save["ros_rate"] = it_cluster->second.as<double>();
			continue;
		}
		//Iterate through parts
		for (YAML::const_iterator it_part = it_cluster->second.begin(); it_part != it_cluster->second.end(); ++it_part)
		{
			const std::string &part = it_part->first.as<std::string>();
			//Iterate through devices
			for (YAML::const_iterator it_dev = it_part->second.begin(); it_dev != it_part->second.end(); ++it_dev)
			{
				const std::string &dev = it_dev->first.as<std::string>();
				//Iterare through properties
				for (YAML::const_iterator it_prop = it_dev->second.begin(); it_prop != it_dev->second.end(); ++it_prop)
				{
					config_save[dev][cluster][part][it_prop->first.as<std::string>()] = it_prop->second.as<uint16_t>();
					//ROS_INFO("Creating param [%s] with value: %d", ros_param_name.str().c_str(), it_prop->second.as<uint16_t>());
				}
			}
		}
	}

	ros::param::set("/pumpkin/config", config_save);

	return 0;
}
