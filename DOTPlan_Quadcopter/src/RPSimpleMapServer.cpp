#include "dotplan_interface_quadrotor/RPSimpleMapServer.h"

#include <tf/transform_listener.h>

/* implementation of dotplan_interface_mapping::RPSimpleMapServer.h */

	/* constructor */
	RPSimpleMapServer::RPSimpleMapServer(ros::NodeHandle &nh){

		// visualisation
		waypoints_pub = nh.advertise<visualization_msgs::MarkerArray>("viz/waypoints", 10, true);
	}

	/*-----------*/
	/* build PRM */
	/*-----------*/

	/**
	 * parses a pose with yaw from strings: "[f, f, f]"
	 */
	 void RPSimpleMapServer::parsePose(geometry_msgs::PoseStamped &pose, std::string line) {

		int curr,next;
		curr=line.find("[")+1;
		next=line.find(",",curr);

		pose.pose.position.x = (double)atof(line.substr(curr,next-curr).c_str());
		curr=next+1; next=line.find(",",curr);

		pose.pose.position.y = (double)atof(line.substr(curr,next-curr).c_str());
		curr=next+1; next=line.find("]",curr);

		float theta = atof(line.substr(curr,next-curr).c_str());
		tf::Quaternion q;
		q.setEuler(theta, 0 ,0);
               


		pose.pose.orientation.x = q.x();
		pose.pose.orientation.y = q.z();
		pose.pose.orientation.w = q.w();
		pose.pose.orientation.z = q.y();
	}

	bool RPSimpleMapServer::setupRoadmap(std::string filename) {

		ros::NodeHandle nh("~");

		// clear previous roadmap from knowledge base
		ROS_INFO("KCL: (RPSimpleMapServer) Loading roadmap from file");

		// load configuration file
		std::ifstream infile(filename.c_str());
		std::string line;
		int curr,next;
		while(std::getline(infile, line)) {
			// read waypoint
			curr=line.find("[");
			std::string name = line.substr(0,curr);

			// data
			geometry_msgs::PoseStamped pose;
			parsePose(pose, line);

			// save here for viz
			Waypoint* wp = new Waypoint(name, pose.pose.position.x, pose.pose.position.y,pose.pose.orientation.w);
			waypoints[wp->wpID] = wp;
		}
		infile.close();

		// publish visualization
                ros::Rate poll_rate(1);
                
		//while(waypoints_pub.getNumSubscribers()==0 && ros::ok()){
                //poll_rate.sleep();  
		//publishWaypointMarkerArray(nh);
		//}
	}

bool RPSimpleMapServer::getwaypoints(dotplan_interface_quadrotor::Get_waypoints::Request  &req,
         dotplan_interface_quadrotor::Get_waypoints::Response &res)
{
  ROS_ERROR("DDDDDD");
  Waypoint* wp = waypoints.find(req.wpID)->second;
  geometry_msgs::PoseStamped waypose;
  waypose.pose.position.x=wp->real_x;
  waypose.pose.position.y=wp->real_y;
  waypose.pose.orientation.z=wp->real_theta;
  res.waypoint_pose=waypose;
  ROS_INFO("request: x=%s", req.wpID.c_str());
  return true;

}


	/*-------------*/
	/* Main method */
	/*-------------*/

	int main(int argc, char **argv) {

		// setup ros
		ros::init(argc, argv, "waypoint_server");
		ros::NodeHandle nh("~");

		// params
		std::string filename("waypoints.txt");
		nh.param("waypoint_file", filename, filename);
        ROS_ERROR(" nome do arquivo %s",filename.c_str());
		// init
		RPSimpleMapServer sms(nh);
		ros::ServiceServer service = nh.advertiseService("/get_waypoints",&RPSimpleMapServer::getwaypoints, &sms);
		sms.setupRoadmap(filename);

		ROS_INFO("KCL: (RPSimpleMapServer) Ready to receive.");
		ros::spin();
		return 0;
	}
