/**
 * @file IfOppoInFront.cpp
 *
 * @param theObstacle 
 * @param theRobotPose
 * @return int 0:no obstacle 1:the obstacle is in the left 2:the obstacle is in the right.
 *
 * @author Xuanzhi Wang
 */
#include "BallModel.h"
#include "Tools/Modeling/Obstacle.h"
#include "Tools/Math/Eigen.h"
#include "RobotPose.h"
#include "Tools/Math/Pose2f.h"
#include "ObstacleModel.h"
#include "IfOppoInFront.h"


int IfOppoInFront(const ObstacleModel& theObstacle , const RobotPose& theRobotPose) {
    unsigned i = 0;
    for ( i = 0; i < theObstacle.obstacles.size(); i++) {
        if (std::abs(ObstacleModelToField(theObstacle.obstacles[i], theRobotPose).translation.x() - 4500.f) > 10.f) {
            if (theObstacle.obstacles[i].type == Obstacle::opponent || theObstacle.obstacles[i].type == Obstacle::someRobot)
                if (std::abs(theObstacle.obstacles[i].center.y() ) < 125) {
                    if (theObstacle.obstacles[i].center.y() < 125.f && theObstacle.obstacles[i].center.y() >=  0.f)
                        return 1;
                    if (theObstacle.obstacles[i].center.y() < 0.f && theObstacle.obstacles[i].center.y() > -125.f)
                        return 2;
                    else return 0;
                }
            if (theObstacle.obstacles[i].center.norm() < 1000.0 ) {
                if (theObstacle.obstacles[i].center.y() < 250.f && theObstacle.obstacles[i].center.y() >= 0.f)
                    return 1;
                if (theObstacle.obstacles[i].center.y() < 0.f && theObstacle.obstacles[i].center.y() > -250.f)
                    return 2;
                else return 0;
            }
        } else return 0;
    }
    return 0;
}



/*

    改代码为之前写的可以调用obstacle的，现在编译有误！
for( i = 0;i < theObstacleModel.obstacles.size();i++)
            {
                if(theObstacleModel.obstacles[i].type == Obstacle::opponent || theObstacleModel.obstacles[i].type == Obstacle::someRobot)          // 具体再改
                {
                    if(theObstacleModel.obstacles[i].center.norm()  <  1200.f )
                    {
                        if(theObstacleModel.obstacles[i].center.y()  > -250.f  && theObstacleModel.obstacles[i].center.y() < 0.f )
                        {   flag_T=2 ; break; }
                        if(theObstacleModel.obstacles[i].center.y()  > 0.f  && theObstacleModel.obstacles[i].center.y() < 250.f )
                        {  flag_T=3;break;}
                    }
                }
            }

  */