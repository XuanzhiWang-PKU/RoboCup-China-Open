#pragma once
#include "Tools/Math/Eigen.h"
#include "RobotPose.h"
#include "Tools/Math/Pose2f.h"

Pose2f AbsolutePointToRobot(const RobotPose &theRobotPose,float Absolute_X,float Absolute_Y);
