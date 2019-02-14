/**
 * @file GetY.h
 *
 * @param theRobotPose 
 * @param velocity  the ball velocity
 * @param x 
 * @return float return y base on the x and 
 *
 * @author Xuanzhi Wang
 */
#include "TeamBallModel.h"
#include "RobotPose.h"
#include "Tools/Math/Pose2f.h"

float GetY(const Pose2f theRobotPose, const Vector2f velocity, const float  x = 4500.f);