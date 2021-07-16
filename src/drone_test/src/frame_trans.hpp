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
using namespace std;


#define CALIB_TIME 30

geometry_msgs::PoseStamped POSE_OFFSET;
float YAW_OFFSET;


void offset_reset()
{
    POSE_OFFSET.pose.position.x = 0;
    POSE_OFFSET.pose.position.y = 0;
    POSE_OFFSET.pose.position.z = 0;
    YAW_OFFSET = 0;
}


void offset_calib(  const std_msgs::Float64ConstPtr &fcu_heading_addr,    \
                    const geometry_msgs::PoseStamped::ConstPtr &fcu_pose_addr
                    )
{

    for (int i = 1; i <= CALIB_TIME; ++i)
    {

        ros::spinOnce();            //处理消息订阅
        ros::Duration(0.1).sleep(); //Sleep 0.1s

        YAW_OFFSET += fcu_heading_addr->data;
        POSE_OFFSET.pose.position.x += fcu_pose_addr->pose.position.x;
        POSE_OFFSET.pose.position.y += fcu_pose_addr->pose.position.y;
        POSE_OFFSET.pose.position.z += fcu_pose_addr->pose.position.z;

        // ROS_INFO("current heading%d: %f", i, GYM_OFFSET / i);
    }
    YAW_OFFSET /= CALIB_TIME;
    POSE_OFFSET.pose.position.x /= CALIB_TIME;
    POSE_OFFSET.pose.position.y /= CALIB_TIME;
    POSE_OFFSET.pose.position.z /= CALIB_TIME;
    // ROS_INFO("the N' axis is facing: %f", GYM_OFFSET);
    // cout << GYM_OFFSET << endl;
    
}




float yaw_fcu2gym(float fcuyaw)
{
    return 
}




//注意Yaw变换！！

geometry_msgs::PoseStamped frame_fcu2gym(geometry_msgs::PoseStamped pose)
{
    pose.pose.position.x -= POSE_OFFSET.pose.position.x;
    pose.pose.position.y -= POSE_OFFSET.pose.position.y;
    pose.pose.position.z -= POSE_OFFSET.pose.position.z;
    return pose;
}

geometry_msgs::PoseStamped frame_gym2fcu(geometry_msgs::PoseStamped pose)
{
    pose.pose.position.x += POSE_OFFSET.pose.position.x;
    pose.pose.position.y += POSE_OFFSET.pose.position.y;
    pose.pose.position.z += POSE_OFFSET.pose.position.z;
    return pose;
}

void frame_fcu2gym(float* x, float* y, float* z)
{
    *x -= POSE_OFFSET.pose.position.x;
    *y -= POSE_OFFSET.pose.position.y;
    *z -= POSE_OFFSET.pose.position.z;
}

void frame_gym2fcu(float* x, float* y, float* z)
{
    *x += POSE_OFFSET.pose.position.x;
    *y += POSE_OFFSET.pose.position.y;
    *z += POSE_OFFSET.pose.position.z;
}