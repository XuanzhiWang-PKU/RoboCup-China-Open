/**
 * @file ObstacleModelToField.h
 *
 * @param theObstacle 
 * @param theRobotPose
 * @return Pose2f
 *
 * @author Xuanzhi Wang
 */
#pragma once
#include "BallModelToField.h"
#include "BallModel.h"
#include "Tools/Math/Eigen.h"
#include "RobotPose.h"
#include "Tools/Math/Pose2f.h"
#include "Tools/Modeling/Obstacle.h"

Pose2f ObstacleModelToField(const Obstacle& theObstacle,const RobotPose& theRobotPose);