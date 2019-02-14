/*** behavior for the ready state */
/**author Colony Albert,last edit 17.5.24*/
/*
  Search the templategenerator.cpp for more information

  this file declears the readystate in which the robot shall walk into the field
    the final position can be redefined below

  after the readystate the robot will face the opponent's gate defaultly
*/
/*
    if you have absulotely confidence that the robot will stand in the right
    position,that means the robot will not adjust after walk in,
    then you can edit the ending component of this file.
*/
option(ReadyState) {
//define some common data
    float commonSpeed = 0.6f;
    float turnSpeed = 1.047f;
    float deltadistance = 20.0f;
    float distance;
    Pose2f relatePoint;
//define the number of the robot
#define STRIKER_NUMBER 1
#define SUPPORTER1_NUMBER 2
#define SUPPORTER2_NUMBER 3
#define DEFENDER_NUMBER 4
#define KEEPER_NUMBER 5
//define the key position
    float STRIKER_X = -400.0f;
    float STRIKER_Y = 0.0f;
    float STRIKER_X_ATTACK = -850.0f;
    float STRIKER_Y_ATTACK = 0.0f;
    float KEEPER_X = -4000.0f;
    float KEEPER_Y = 0.0f;
    float DEFENDER_X = -2500.0f;
    float DEFENDER_Y = -1500.0f;
    float SUPPORTER1_X = -1500.0f;
    float SUPPORTER1_Y = -1500.0f;
    float SUPPORTER2_X = -1500.0f;
    float SUPPORTER2_Y = 1500.0f;
    //float OPPONENTGATE_X = 4500.0f;
    //float OPPONENTGATE_Y = 0.0f;
    float x, y; //commom position for different position
    switch (theRobotInfo.number) {
    case 1: {
        if (theGameInfo.kickOffTeam == theOwnTeamInfo.teamNumber)
            x = STRIKER_X_ATTACK, y = STRIKER_Y_ATTACK;
        else x = STRIKER_X, y = STRIKER_Y;
    } break;
    case 2: x = SUPPORTER1_X, y = SUPPORTER1_Y; break;
    case 3: x = SUPPORTER2_X, y = SUPPORTER2_Y; break;
    case 4: x = DEFENDER_X, y = DEFENDER_Y; break;
    case 5: x = KEEPER_X, y = KEEPER_Y; break;
    default: ;
    }
//state
    initial_state(turnToPosition) {
        transition {
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, x, y);
            if (std::abs(relatePoint.rotation) < 2_deg)
                goto walkToTarget_Ready;
        }
        action {
            LookForward();
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, x, y);
            WalkToTarget(Pose2f(turnSpeed, 0.1f, 0.1f), Pose2f(relatePoint.rotation, 0.0, 0.0));
        }
    }
    state(walkToTarget_Ready) {
        transition {
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, x, y);
            distance = relatePoint.translation.norm();
            if (distance < deltadistance)
                goto alignToGoal;
            if ((relatePoint.rotation > 20_deg) && (distance > 10 * deltadistance))
                goto turnToPosition;
            if (state_time > 10000)
                goto turnToPosition;
        }
        action {
            //LookLeftAndRight();
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, x, y);
            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed), Pose2f(0.0f, relatePoint.translation.x(), relatePoint.translation.y()));
        }
    }
    state(alignToGoal) {
        transition {
            if (std::abs(theRobotPose.rotation) < 1_deg)
                goto adjust;
        }
        action {
            LookForward();
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, x, y);
            WalkToTarget(Pose2f(turnSpeed, 0.1f, 0.1f), Pose2f(-theRobotPose.rotation, 0.0, 0.0));
        }
    }
    state(adjust) {
        transition {
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, x, y);
            distance = relatePoint.translation.norm();
            if (distance > (10 * deltadistance))
                goto walkToTarget_Ready;
        }
        action {
            LookLeftAndRight();
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, x, y);
            distance = relatePoint.translation.norm();
            Stand();
        }
    }
    /*
        target_state(finished)
        {
            transition
            {
                ;
            }
            action
            {
                ShowRobotPose(theRobotPose);
                LookForward();
                Stand();
            }
        }*/
}