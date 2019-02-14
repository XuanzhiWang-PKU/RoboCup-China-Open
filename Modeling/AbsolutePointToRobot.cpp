/**
 *  用于将机器人要到达的绝对坐标转化成相对于机器人的坐标。用WalkToTarget ，裁判盒上场需要
 *  @author: Xuanzhi Wang
 */
#include "BallModel.h"
#include "Tools/Math/Eigen.h"
#include "Tools/Math/Pose2f.h"
#include "RobotPose.h"
#include "AbsolutePointToRobot.h"

Pose2f AbsolutePointToRobot(const RobotPose& theRobotPose, float Absolute_X, float Absolute_Y) {
    float a = Absolute_X - theRobotPose.translation.x();
    float b = Absolute_Y - theRobotPose.translation.y();
    float Relate_X = a * std::cos(theRobotPose.rotation) + b * std::sin(theRobotPose.rotation);
    float Relate_Y = b * std::cos(theRobotPose.rotation) - a * std::sin(theRobotPose.rotation);
    float rot2robot = std::atan2(Relate_Y, Relate_X);
    Vector2f relate(Relate_X, Relate_Y);
    Pose2f RobotPosition(rot2robot, relate);
    return RobotPosition;
}