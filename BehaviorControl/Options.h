/** All option files that belong to the current behavior have to be included by this file. */

#include "Options/Soccer.h"


#include "Options/GameControl/HandleGameState.h"
#include "Options/GameControl/HandlePenaltyState.h"
#include "Options/GameControl/PlayingState.h"
#include "Options/GameControl/ReadyState.h"

#include "Options/HeadControl/HeadControl.h"
#include "Options/HeadControl/LookForward.h"
#include "Options/HeadControl/LookLeftAndRight.h"
#include "Options/HeadControl/FocusBall.h"

#include "Options/Output/Activity.h"
#include "Options/Output/Annotation.h"

#include "Options/Output/ShowParameter/ShowRobotPose.h"
#include "Options/Output/ShowParameter/ShowBallPose.h"
#include "Options/Output/ShowParameter/ShowObstacleModel.h"
#include "Options/Output/ShowParameter/ShowTeamBallPose.h"
#include "Options/Output/ShowParameter/ShowBallToRobot.h"

#include "Options/Output/HeadMotionRequest/SetHeadPanTilt.h"
#include "Options/Output/MotionRequest/InWalkKick.h"
#include "Options/Output/MotionRequest/SpecialAction.h"
#include "Options/Output/MotionRequest/Stand.h"
#include "Options/Output/MotionRequest/WalkAtAbsoluteSpeed.h"
#include "Options/Output/MotionRequest/WalkAtRelativeSpeed.h"
#include "Options/Output/MotionRequest/WalkToTarget.h"
#include "Options/Output/MotionRequest/PathToTarget.h"
#include "Options/Output/MotionRequest/WalkToTargetInPathMode.h"
#include "Options/Output/MotionRequest/GetUpEngine.h"
#include "Options/Output/MotionRequest/Kick.h"
#include "Options/Output/MotionRequest/SearchForBallLeft.h"
#include "Options/Output/MotionRequest/SearchForBallRight.h"
#include "Options/Output/MotionRequest/SearchForBall.h"
#include "Options/Output/MotionRequest/SearchForBall_1.h"
#include "Options/Output/MotionRequest/SearchForBall_2.h"
#include "Options/Output/MotionRequest/SearchForBall_3.h"
#include "Options/Output/MotionRequest/SearchForBall_4.h"
#include "Options/Output/MotionRequest/SearchForBall_5.h"
#include "Options/Output/MotionRequest/InPosiFocusBall.h"
#include "Options/Output/PlaySound.h"

#include "Options/Roles/Striker.h"
#include "Options/Roles/StrikerFirst.h"
#include "Options/Roles/Supporter.h"
#include "Options/Roles/SupporterFirst_1.h"
#include "Options/Roles/SupporterFirst_2.h"
#include "Options/Roles/Keeper.h"
#include "Options/Roles/Defender.h"
#include "Options/Roles/Penalty_shoot.h"
#include "Options/Roles/Penalty_keeper.h"

#include "Options/Skills/GetUp.h"
#include "Options/Skills/AlignToGoalFromRight.h"


#include "Options/Tools/ButtonPressedAndReleased.h"