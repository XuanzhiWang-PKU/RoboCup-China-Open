/**
 * @file GetAreaNumber.cpp
 *
 * @param theTeamBallModel
 * @param theRobotPose
 *
 * @author Xuanzhi Wang
 */
#include "BallModelToField.h"
#include "RobotPose.h"
#include "Tools/Math/Pose2f.h"
#include "Tools/Math/Eigen.h"
#include "GetAreaNumber.h"
#include "TeamBallModel.h"

// Pose2f BallModel2Field(const BallModel& theballModel, const RobotPose& theRobotPose);
int GetAreaNumber(const TeamBallModel& theTeamBallModel, const RobotPose& theRobotPose) {
    //Pose2f BallModel = BallModel2Field(theballModel, theRobotPose);
    if (theTeamBallModel.position.x() >= 0.f)
        return 1;
    if (theTeamBallModel.position.x() < 0.f && theTeamBallModel.position.x() >= -3200.f)
        return 2;
    if (theTeamBallModel.position.x() < -3200.f && theTeamBallModel.position.x() > -4500.f)
        return 3;
}