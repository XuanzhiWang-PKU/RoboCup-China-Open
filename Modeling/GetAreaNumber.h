/**
 * @file GetAreaNumber.h
 *
 * @param theTeamBallModel 
 * @param theRobotPose
 *
 * @author Xuanzhi Wang
 */
#pragma once
#include "BallModelToField.h"
#include "RobotPose.h"
#include "Tools/Math/Pose2f.h"
#include "Tools/Math/Eigen.h"
#include "TeamBallModel.h"

int GetAreaNumber(const  TeamBallModel& theTeamBallModel, const RobotPose& theRobotPose);