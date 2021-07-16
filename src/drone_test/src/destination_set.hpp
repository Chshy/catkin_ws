#include <iostream>
#include <ros/ros.h> 
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>
#include "std_msgs/Float64.h"
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/PositionTarget.h>
#include <unistd.h>
#include <geometry_msgs/Pose2D.h>
#include <mavros_msgs/CommandTOL.h>
#include <time.h>
#include <cmath>
#include <math.h>
#include <ros/duration.h>

#include "frame_trans.hpp"

using namespace std;

geometry_msgs::PoseStamped destinatin_gym;
geometry_msgs::PoseStamped destinatin_fcu;




//设置飞机的朝向
//set orientation of the drone (drone should always be level)
void setYaw_fcu(float heading)
{
    float yaw = heading * (M_PI / 180);
    float pitch = 0;
    float roll = 0;
    float cy = cos(yaw * 0.5);
    float sy = sin(yaw * 0.5);
    float cr = cos(roll * 0.5);
    float sr = sin(roll * 0.5);
    float cp = cos(pitch * 0.5);
    float sp = sin(pitch * 0.5);
    float qw = cy * cr * cp + sy * sr * sp;
    float qx = cy * sr * cp - sy * cr * sp;
    float qy = cy * cr * sp + sy * sr * cp;
    float qz = sy * cr * cp - cy * sr * sp;
    destinatin_fcu.pose.orientation.w = qw;
    destinatin_fcu.pose.orientation.x = qx;
    destinatin_fcu.pose.orientation.y = qy;
    destinatin_fcu.pose.orientation.z = qz;
}

void setYaw_gym(float heading)
{
    setYaw_fcu(-heading + 90 - YAW_OFFSET);
}


void setDestination_fcu(float x, float y, float z)
{
    destinatin_fcu.pose.position.x = x;
    destinatin_fcu.pose.position.y = y;
    destinatin_fcu.pose.position.z = z;
    float X = x;
    float Y = y;
    float Z = z;
    frame_gym2fcu(&X,&Y,&Z);
    destinatin_gym.pose.position.x = X;
    destinatin_gym.pose.position.y = Y;
    destinatin_gym.pose.position.z = Z;
}


// set position to fly to in the gym frame
void setDestination_gym(float x, float y, float z)
{
    destinatin_gym.pose.position.x = x;
    destinatin_gym.pose.position.y = y;
    destinatin_gym.pose.position.z = z;
    float X = x;
    float Y = y;
    float Z = z;
    frame_gym2fcu(&X,&Y,&Z);
    destinatin_fcu.pose.position.x = X;
    destinatin_fcu.pose.position.y = Y;
    destinatin_fcu.pose.position.z = Z;
}
