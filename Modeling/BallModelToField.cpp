/**
 *  用于将机器人要到达的相对于机器人的坐标转化成绝对坐标。
 *  @author: Xuanzhi Wang
 */
#include "BallModelToField.h"
#include "BallModel.h"
#include "Tools/Math/Eigen.h"
#include "Tools/Math/Pose2f.h"
#include "RobotPose.h"

Pose2f BallModel2Field(const BallModel& theBallModel,const RobotPose& theRobotPose)
{
    float  ballrot = std::atan(theBallModel.estimate.position.y() /theBallModel.estimate.position.x() );   //球相对机器人的旋转角
    float ballrot2ground = theRobotPose.rotation + ballrot;    //球相对球场的旋转角

    float c =  theBallModel.estimate.position.norm();              //球相对机器人坐标系的长度(x^2+y^2)
    //把机器人坐标系旋转后，球相对机器人坐标系的坐标
    float Ball_X = std::cos(ballrot2ground) * c;
    float Ball_Y = std::sin(ballrot2ground) * c;
    //球相对于球场的坐标
    Ball_X = Ball_X + theRobotPose.translation.x();
    Ball_Y = Ball_Y + theRobotPose.translation.y();
     Vector2f goalpos(-4500,0);
    float  rot = std::atan(Ball_Y/(Ball_X - goalpos.x()));    
    Vector2f ballpos(Ball_X,Ball_Y);
    
    Pose2f newballpos(rot,ballpos);
    return newballpos;
}