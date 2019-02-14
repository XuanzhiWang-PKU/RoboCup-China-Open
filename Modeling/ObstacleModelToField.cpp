/**
 * @file ObstacleModelToField.cpp
 *
 * @param theObstacle 
 * @param theRobotPose
 * @return Pose2f
 *
 * @author Xuanzhi Wang
 */
#include "ObstacleModelToField.h"
#include "BallModel.h"
#include "Tools/Math/Eigen.h"
#include "Tools/Math/Pose2f.h"
#include "RobotPose.h"

Pose2f ObstacleModelToField(const Obstacle& theObstacle, const RobotPose& theRobotPose) {
    float  obstaclerot = std::atan(theObstacle.center.y() / theObstacle.center.x() );  //球相对机器人的旋转角
    float obstaclerot2ground = theRobotPose.rotation + obstaclerot;    //球相对球场的旋转角
    float c =  theObstacle.center.norm();              //球相对机器人坐标系的长度(x^2+y^2)
    //把机器人坐标系旋转后，球相对机器人坐标系的坐标
    float Obstacle_X = std::cos(obstaclerot2ground) * c;
    float Obstacle_Y = std::sin(obstaclerot2ground) * c;
    //球相对于球场的坐标
    Obstacle_X = Obstacle_X + theRobotPose.translation.x();
    Obstacle_Y = Obstacle_Y + theRobotPose.translation.y();
    Vector2f goalpos(-4500, 0);
    float  rot = std::atan(Obstacle_Y / (Obstacle_X - goalpos.x()));
    Vector2f obstaclePos(Obstacle_X, Obstacle_Y);
    Pose2f newObstaclePos(rot, obstaclePos);
    return newObstaclePos;
}