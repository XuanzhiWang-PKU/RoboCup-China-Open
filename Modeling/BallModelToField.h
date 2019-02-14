#pragma once
#include "BallModelToField.h"
#include "BallModel.h"
#include "Tools/Math/Eigen.h"
#include "RobotPose.h"
#include "Tools/Math/Pose2f.h"

Pose2f BallModel2Field(const BallModel& theballModel,const RobotPose& theRobotPose);