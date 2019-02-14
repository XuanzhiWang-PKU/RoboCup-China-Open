/**
 * @file IfOppoInFront.h
 *
 * @param theObstacle 
 * @param theRobotPose
 * @return int 0:no obstacle 1:the obstacle is in the left 2:the obstacle is in the right.
 *
 * @author Xuanzhi Wang
 */
#pragma	once
#include "Tools/Modeling/Obstacle.h"
#include "BallModel.h"
#include "Tools/Math/Eigen.h"
#include "RobotPose.h"
#include "Tools/Math/Pose2f.h"
#include "ObstacleModelToField.h"

int IfOppoInFront(const ObstacleModel& theObstacle ,const RobotPose& theRobotPose);