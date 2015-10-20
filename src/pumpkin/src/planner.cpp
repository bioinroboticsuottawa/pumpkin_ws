#include <ros/ros.h>
#include <pluginlib/class_loader.h>
#include <yaml-cpp/yaml.h>

#include <actionlib/client/simple_action_client.h>
#include <actionlib/server/simple_action_server.h>
#include "pumpkin_messages/PlaybackAction.h"
#include "pumpkin_messages/SceneAction.h"
#include "pumpkin_messages/analog_array.h"
#include "pumpkin_messages/SSCMoveList.h"
#include "file_type.h"

#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/joint_model.h>
#include <moveit/planning_interface/planning_interface.h>
#include <moveit/planning_scene/planning_scene.h>
#include <moveit/kinematic_constraints/utils.h>
#include <moveit_msgs/DisplayTrajectory.h>

namespace pmsg = pumpkin_messages;

typedef actionlib::SimpleActionClient<pmsg::PlaybackAction> PlaybackClient;
typedef actionlib::SimpleActionServer<pmsg::SceneAction> SceneServer;

enum class SceneState {
	Stopped,
	Moving,
	Preparing,
	Switching,
};

struct auxiliar_calibration {
	int ssc_min;
	int ssc_max;
	int ssc_pin;
	double joint_min;
	double joint_max;
};

SceneState state;
std::vector<std::string> playback_list;
std::vector<double> joint_positions;
std::map<std::string, auxiliar_calibration> joint_map;
pmsg::SceneFeedback feedback;
pmsg::SceneResult result;
uint32_t atual_step;

std::vector<std::string> joint_names;
std::vector<std::vector<double> > joint_steps;

ros::Subscriber playback;
ros::Publisher ssc;

void onGoal(SceneServer& server) {
	if (state != SceneState::Stopped) {
		ssc.publish(pmsg::SSCMoveList());
	}
	auto goal = server.acceptNewGoal();
	playback_list.swap(goal->filenames);
	atual_step = 0;
}

void onPreempt(SceneServer& server, PlaybackClient &client) {
	if (state == SceneState::OnMovement) {
		client.cancelGoal();
		result.state = client.getResult()->state;
	} else if (state == SceneState::OnIntermediate) {
		ssc.publish(pmsg::SSCMoveList());
		result.state = static_cast<uint8_t>(pmsg::IOState::OK);
	}
	server.setAborted(result);
}

void playbackMoves(sensor_msgs::JointStateConstPtr &msg) {
	if (joint_names.empty()) {
		joint_names = msg->name;
	}
	joint_steps.push_back(msg->position);
}

int main (int argc, char *argv[]) {
	ros::init(argc, argv, "pumpkin_planner");
	ros::NodeHandle nh;

	while (!ros::param::has("/pumpkin/config")) {
		ros::Duration(1, 0).sleep();
	}

	PlaybackClient client("playback_action", false);
	SceneServer server("scene_action", false);

	server.registerGoalCallback(boost::bind(&onGoal, server));
	server.registerPreemptCallback(boost::bind(&onPreempt, server, client));

	state = SceneState::Stopped;
	atual_step = 0;

	playback = nh.subscribe("playback_joints", 32, &playbackMoves);
	ssc = nh.advertise("move_ssc_topic", 32);

	client.waitForServer();

	//Create the pumpkin model
	robot_model_loader::RobotModelLoader pumpkinModelLoader("pumpkin_description");
	robot_model::RobotModelPtr pumpkinModel = pumpkinModelLoader.getModel();

	//Create the planning scene, that coordinates the objects in the world
	planning_scene::PlanningScenePtr planningScene(new planning_scene::PlanningScene(pumpkinModel));

	boost::scoped_ptr<pluginlib::ClassLoader<planning_interface::PlannerManager> > plannerLoader;
	planning_interface::PlannerManagerPtr planner;
	std::string plannerName;

	if (!nh.getParam("planning_plugin", plannerName))
		ROS_FATAL_STREAM("Could not find planner plugin name");
	try
	{
		plannerLoader.reset(new pluginlib::ClassLoader<planning_interface::PlannerManager>("moveit_core", "planning_interface::PlannerManager"));
	}
	catch(pluginlib::PluginlibException& ex)
	{
		ROS_FATAL_STREAM("Exception while creating planning plugin loader " << ex.what());
	}
	try
	{
		planner.reset(plannerLoader->createUnmanagedInstance(plannerName));
		if (!planner->initialize(pumpkinModel, nh.getNamespace()))
			ROS_FATAL_STREAM("Could not initialize planner instance");
		ROS_INFO_STREAM("Using planning interface '" << planner->getDescription() << "'");
	}
	catch(pluginlib::PluginlibException& ex)
	{
		const std::vector<std::string> &classes = plannerLoader->getDeclaredClasses();
		std::stringstream ss;
		for (std::size_t i = 0 ; i < classes.size() ; ++i)
			ss << classes[i] << " ";
		ROS_ERROR_STREAM("Exception while loading planner '" << plannerName << "': " << ex.what() << std::endl
		                 << "Available plugins: " << ss.str());
	}

	//So far... we have the planner (by default it should be OMPL)
	moveit_msgs::MotionPlanRequest req;
	moveit_msgs::MotionPlanResponse res;

	XmlRpc::XmlRpcValue sscConf;
	ros::param::get("/pumpkin/config/ssc", sscConf);

	//Build que calibration parameters
	typedef std::vector<moveit::core::JointModel *> JointVector;
	for (XmlRpc::XmlRpcValue::iterator sscClusterIt = sscConf.begin(); sscClusterIt != sscConf.end(); ++sscClusterIt) {
		auto joints = pumpkinModel->getJointModelGroup(sscClusterIt->first)->getJointModels();
		for(JointVector::iterator jointIt = joints.begin(); jointIt != joints.end(); ++jointIt) {
			const std::string &name = (*jointIt)->getName();
			const std::string &part_name = name.substr(2);
			auto bounds = (*jointIt)->getVariableBounds[0];
			joint_map.insert(std::pair(name, auxiliar_calibration {
										  int(sscClusterIt->second[part_name]["pulse_min"]),		//SSC min
										  int(sscClusterIt->second[part_name]["pulse_max"]),		//SSC max
										  int(sscClusterIt->second[part_name]["pin"]),				//SSC pin
										  bounds.min_position,										//Joint min
										  bounds.max_position,										//Joint max
									  }));
		}
	}

	double rate;
	if (!ros::param::get("/pumpkin/config/ros_rate", rate))
		rate = 100;
	ros::Rate ros_rate(rate);

	while (ros::ok()) {
		if (!server.isActive())
			continue;
		switch (state) {
			case SceneState::Switching:

			break;
			case SceneState::Preparing:

			break;
			case SceneState::Moving:

			case SceneState::Stopped:
				ros_rate.sleep();
			break;
		}
	}

	return 0;
}