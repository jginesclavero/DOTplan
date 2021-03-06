//=================================================================================================
// Copyright (c) 2012, Johannes Meyer, TU Darmstadt
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Flight Systems and Automatic Control group,
//       TU Darmstadt, nor the names of its contributors may be used to
//       endorse or promote products derived from this software without
//       specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//=================================================================================================
#include <iostream>
#include <ctype.h>
#include <sstream>
#include <string>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <ros/ros.h>
#include <std_srvs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/Range.h>
#include <tf/LinearMath/Quaternion.h>
#include <tf/LinearMath/Vector3.h>
#include <tf/transform_datatypes.h>
//#include <hector_nav_msgs/GetRobotTrajectory.h>
#include "actionlib_msgs/GoalStatus.h"
#include "actionlib_msgs/GoalID.h"
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include "std_msgs/String.h"
#include "dotplan_interface_quadrotor/Action_interface.h"
#include "dotplan_interface_quadrotor/Get_waypoints.h"
#include "nav_msgs/Odometry.h"
#include <boost/algorithm/string.hpp>
#include "xdot/Action_feedback.h"
#include "xdot/Action_interface.h"
#include "xdot/RequestReloadFiles.h"
namespace dotplan_interface_quadrotor
    {
    typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
    class Quadrotor
        {
    private:
        ros::NodeHandle node_handle_;


        // motor interface
        ros::ServiceClient motor_service_client;


        // topic for publishing action feedback
        ros::Publisher feedback_publisher_;

        // subscription to receive drone height messages
        ros::Subscriber height_subscriber_;

        // subscription to receive drone height messages
        ros::Subscriber missionstatus_subscriber_;

        // topic for publishing geometry_msgs::Twist data
        ros::Publisher velocity_publisher_;

        // topic for publishing geometry_msgs::Twist data
        ros::Publisher interaction_publisher_;

        // published velocity
        geometry_msgs::Twist velocity_;

        // published velocity
        geometry_msgs::PoseStamped waypoint;

        ros::ServiceClient clear_costmaps_client;



        float height_;
        double Yaw;
        int  missionstatus_;



        void clearcostmap(){

                std_srvs::Empty emptySrv;
                clear_costmaps_client.call(emptySrv);
            }

    public:

     //   void takeoff(std::vector<std::string> currentActionParams, std::string &outcome, bool &succeed);
     //   void land(std::vector<std::string> currentActionParams, std::string &outcome, bool &succeed);
     //   void flysquare(std::vector<std::string> currentActionParams, std::string &outcome, bool &succeed);
     //   void pickup(std::vector<std::string> currentActionParams, std::string &outcome, bool &succeed);
     //   void drop(std::vector<std::string> currentActionParams, std::string &outcome, bool &succeed);
     //   void opendoor(std::vector<std::string> currentActionParams, std::string &outcome, bool &succeed);


        Quadrotor(ros::NodeHandle &nh, std::string &actionserver) : node_handle_(nh)
            {


                motor_service_client = nh.serviceClient<std_srvs::Empty>("/shutdown");

                velocity_publisher_ = node_handle_.advertise<geometry_msgs::Twist>("cmd_vel", 10);

                interaction_publisher_ = node_handle_.advertise<dotplan_interface_quadrotor::Action_interface>("/interaction", 10);

                clear_costmaps_client = nh.serviceClient<std_srvs::Empty>("/move_base/clear_costmaps");

                feedback_publisher_ = node_handle_.advertise<xdot::Action_feedback>("/actionFeedback", 10, true);

                height_ = -1;
                srand (time(NULL));
                ROS_WARN("Init");
            }

        ~Quadrotor()
            {
                stop();
            }

        // --- ACTION DISPATCH --
        void dispatchCallback(const xdot::Action_interface::ConstPtr& msg)
            {
                ROS_WARN("Quad: received action");
                // parse action message
                if((msg->state_id<0)||(msg->action_name.length()==0))
                    return;
                std::string actionName = msg->action_name;
                std::vector<std::string> outcomeList;

                //Getting outcomes

                for (int i=0; i < msg->outcomes.size(); ++i)
                {
					outcomeList.push_back(msg->outcomes[i]);
                }

                //Separating action params

                std::vector<std::string> currentActionParams;
                boost::split(currentActionParams,actionName,boost::is_any_of("_"));
                for (std::vector<std::string>::iterator it = currentActionParams.begin(); it != currentActionParams.end(); ++it)
                {
                    std::string param = *it;
                }


                bool succeed = false;
                int outcome = 0;

                //Relating ation name to function
                //In the future will use function mapping as
                //typedef std::map<std::string, math_method_t> math_func_map_t;
                ROS_WARN("III %s",currentActionParams[0].c_str());
                if(currentActionParams[0].compare("takeoff") == 0)
                {
                   takeoff(currentActionParams,outcome,succeed);
                } else if (currentActionParams[0].compare("land") == 0)
                {
                    land(currentActionParams,outcome,succeed);
                } else if (currentActionParams[0].compare("move-to") == 0)
                {
                    moveto(currentActionParams,outcome,succeed);
                }else if (currentActionParams[0].compare("pickup") == 0)
                {
                    pickup(currentActionParams,outcome,succeed);
                }else if (currentActionParams[0].compare("drop") == 0)
                {
                   drop(currentActionParams,outcome,succeed);
                }
                else if (currentActionParams[0].compare("refuel") == 0){
                   refuel(currentActionParams,outcome,succeed);
                }
                else if (currentActionParams[0].compare("opendoor") == 0){
                   opendoor(currentActionParams,outcome,succeed);
                }

                //Publlish Feedback

                ros::Duration(0.5).sleep();
                xdot::Action_feedback fb;
                fb.state_name=msg->state_name;
                fb.action_name=msg->action_name;
                fb.state_id=msg->state_id;
                fb.outcome=outcomeList[outcome];
                fb.succeed=succeed;
                feedback_publisher_.publish(fb);
            }


        void opendoor(std::vector<std::string> currentActionParams, int &outcome, bool &succeed){

                ROS_INFO("Dispatching opendoor action.");

                // publish message to opendoor

                dotplan_interface_quadrotor::Action_interface msg;
                msg.action_name="opendoor";
                msg.model_name="";
                interaction_publisher_.publish(msg);
                ros::spinOnce();
                ROS_INFO("Action opendoor dispatched.");
                succeed=true;
                int iSecret = rand() % 10 + 1;
                if(iSecret <= 9){ //60%
                  outcome=0;
                }
                else{
                    outcome=1;
                }



            }

        void pickup(std::vector<std::string> currentActionParams, int &outcome, bool &succeed)
            {
                ros::Rate loop_rate(0.25);
                ROS_INFO("Dispatching pickup  action.");

                // publish message to pickup

                dotplan_interface_quadrotor::Action_interface msg;
                msg.action_name=currentActionParams[0];
                msg.model_name=currentActionParams[2];
                interaction_publisher_.publish(msg);
                ros::spinOnce();
                ROS_INFO("Action pickup dispatched.");
                succeed=true;
                outcome=0;


            }

       void refuel(std::vector<std::string> currentActionParams, int &outcome, bool &succeed)
        {
		ROS_INFO("Dispatching refuel action.");
                ROS_INFO("Action refuel dispatched.");
                ros::Duration(1.5).sleep();
                succeed=true;
                outcome=0;
        }

        void drop(std::vector<std::string> currentActionParams, int &outcome, bool &succeed)
            {
                ROS_INFO("Dispatching drop action.");
                dotplan_interface_quadrotor::Action_interface msg;
                msg.action_name=currentActionParams[0];
                msg.model_name=currentActionParams[2];
                interaction_publisher_.publish(msg);
                ros::spinOnce();
                ROS_INFO("Action drop dispatched.");
                succeed=true;
                outcome=0;

            }

        void takeoff(std::vector<std::string> currentActionParams, int &outcome, bool &succeed)
            {

                ROS_INFO("Dispatching takeoff action.");

                // publish message to take off

                velocity_ = geometry_msgs::Twist();
                velocity_.linear.z = 1.0;
                velocity_publisher_.publish(velocity_);

                while(height_ < 0.7)
                {
                    ros::spinOnce();
                }

                // publish message to stop
                stop();
                ROS_INFO("Action takeoff dispatched.");
                clearcostmap();
                succeed=true;
                outcome=0;

            }

        void land(std::vector<std::string> currentActionParams, int &outcome, bool &succeed)
            {

                ROS_INFO("Dispatching land action.");
                // publish message to start descent
                velocity_ = geometry_msgs::Twist();
                velocity_.linear.z = -0.3;
                velocity_publisher_.publish(velocity_);

                // wait until the drone touches the ground
                while(height_ > 0.175)
                {
                    ros::spinOnce();
                }
                ROS_INFO("Landed");

                // stop drone and shutdown motors
                emergency();

                succeed=true;
                outcome=0;
            }

        void moveto(std::vector<std::string> currentActionParams, int &outcome, bool &succeed)
            {
                ROS_WARN("IIITTT %s",currentActionParams[3].c_str());
                ros::Rate loop_rate(0.25);
                //tell the action client that we want to spin a thread by default
                MoveBaseClient ac("move_base", true);

                //wait for the action server to come up
                while(!ac.waitForServer(ros::Duration(5.0))){
                    ROS_INFO("Waiting for the move_base action server to come up");
                }

                move_base_msgs::MoveBaseGoal goal = getWaypointCoordinates(currentActionParams[3]);
                ROS_INFO("Going to waypoint %s",currentActionParams[3].c_str());

                ROS_INFO("Sending goal");
                ac.sendGoal(goal);

                ac.waitForResult();
                ROS_INFO("Termino");

                stop();
                ROS_INFO("Action gotowaypoint dispatched.");
                if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){

                    ROS_ERROR("move succed");
                    succeed=true;
                    outcome=0;
                    /*int iSecret = rand() % 10 + 1;
                    if(iSecret <= 6){ //60%
                    	outcome=0;
                    }
                    else{
                        outcome=1;
                    }*/
                }
                else{
                    rotate();
                    succeed=false;
                    ROS_ERROR("move failed");

                }
            }

        void rotate(){
                ros::Rate loop_rate(0.2);
                velocity_ = geometry_msgs::Twist();
                velocity_.angular.z = -0.1;
                velocity_publisher_.publish(velocity_);
                loop_rate.sleep();


            }


        move_base_msgs::MoveBaseGoal getWaypointCoordinates(std::string waypointID){

                move_base_msgs::MoveBaseGoal goal;
                // header
                ROS_ERROR("IDDDDD");
                goal.target_pose.header.frame_id = "map";
                goal.target_pose.header.stamp = ros::Time::now();

                ros::ServiceClient gws_c = node_handle_.serviceClient<dotplan_interface_quadrotor::Get_waypoints>("/get_waypoints");
                dotplan_interface_quadrotor::Get_waypoints getway;
                getway.request.wpID =waypointID;
                gws_c.call(getway);
                goal.target_pose.pose=getway.response.waypoint_pose.pose;

                ROS_ERROR("ID %s",waypointID.c_str());
                ROS_ERROR("POSE X %f",goal.target_pose.pose.position.x);
                ROS_ERROR("POSE Y %f",goal.target_pose.pose.position.y);
                ROS_ERROR("POSE T %f",goal.target_pose.pose.orientation.z);
                return goal;

            }

        // process messages received from sonar
        // -> range = 0.17 if landed
        void heightCallback(const sensor_msgs::Range::ConstPtr& msg)
            {
                // update quadrotor height
                height_ = msg->range;

            }

        void orientationCallback(const nav_msgs::Odometry::ConstPtr& msg)
            {
                // update quadrotor height
                geometry_msgs::Quaternion orientation_ = msg-> pose.pose.orientation;


            }


        void emergency()
            {
                // publish message to stop
                stop();
                // shutdown motors
                shutdown();
            }

        void stop()
            {
                if(velocity_publisher_.getNumSubscribers() > 0)
                {
                    velocity_ = geometry_msgs::Twist();
                    velocity_publisher_.publish(velocity_); // sending stop message
                }
            }

        void shutdown()
            {
                // call shutdown service with empty service to shutdown motors
                std_srvs::Empty empty;
                motor_service_client.call(empty);
            }

        };

    }


int main(int argc, char **argv)
    {
        ros::init(argc, argv, "dotplan_interface_quadrotor");
        ros::NodeHandle nh;
        ROS_ERROR("Ta no novo");

        // initialize actionserver name by request to ROS parameter server
        std::string actionserver;
        nh.param("nav_server", actionserver, std::string("/nav_server"));

        dotplan_interface_quadrotor::Quadrotor quad_iface(nh,actionserver);

        ros::Subscriber action_sub = nh.subscribe("/actionDispatch", 10, &dotplan_interface_quadrotor::Quadrotor::dispatchCallback, &quad_iface);

        ros::Subscriber height_sub = nh.subscribe("/sonar_height", 1000, &dotplan_interface_quadrotor::Quadrotor::heightCallback, &quad_iface);


        ros::spin();

        return 0;
    }
