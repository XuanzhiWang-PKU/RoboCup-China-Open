/**
 * @file GetY.h
 *
 * @param theTeamBallModel 
 * @param theRobotPose
 * @return float
 *
 * @author Xuanzhi Wang
 */
#include "GetY.h"
float GetY(const Pose2f theRobotPose, const Vector2f velocity, const float  x ){
	float y = theRobotPose.translation.y()-(theRobotPose.translation.x()-x)*velocity.y()/velocity.x();
	return y;
}