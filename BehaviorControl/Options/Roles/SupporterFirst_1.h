/**
 * @file SupporterFirst.h
 *
 * If we start the game, firstly, Supporter go to the exact position(X, Y) directly to
 * wait the ball which the Striker pass. SWhen this part done, goto the nomal Supporter.
 * 
 * @author Xuanzhi Wang
 */
#include <stdio.h>
#define PI 3.1415926
#define SPEED 0.3f
#define GOAL_Y 600.f
#define X 1500.f
#define Y -1500.f
option(SupporterFirst_1) {
    initial_state(start) {
        transition {
            if (theGameInfo.kickOffTeam == theOwnTeamInfo.teamNumber && state_time > 2000)
                goto  turnToExactPosition;
            else goto guard;
        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            Stand();
        }
    }
    // initial_state(start) {
    //     transition {

    //         goto turnToExactPosition;

    //     }
    //     action {
    //         theHeadControlMode = HeadControl::focusBall;
    //         Stand();
    //     }
    // }
    state(guard) {
        transition{
            if (state_time > 10000 ||
            theBallModel.estimate.velocity.x() != 0.0 ||
            theBallModel.estimate.velocity.y() != 0.0  )
                goto supporter;
        }
        action{
            theHeadControlMode = HeadControl::focusBall;
            Stand();
        }
    }
    state(supporter) {
        transition{}
        action{
            Supporter();
        }
    }
    state(turnToExactPosition) {
        transition {

            if (std::abs(AbsolutePointToRobot(theRobotPose,  X, Y).rotation) < 5_deg)
                goto walkToExactPosition;
        }
        action {
            //theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(SPEED, SPEED, SPEED),
            Pose2f(AbsolutePointToRobot(theRobotPose,  X, Y).rotation,
            0.f,
            0.f));
        }
    }
    state(walkToExactPosition) {
        transition{
            if (std::abs(theRobotPose.translation.x() - X) < 50.f && std::abs(theRobotPose.translation.y() - Y) < 50.f)
                goto alignToExactPosition;
        }
        action{
            WalkToTargetInPathMode(Pose2f(SPEED, SPEED, SPEED),
            Pose2f(0.f,
             AbsolutePointToRobot(theRobotPose,  X, Y).translation.x(), 
             AbsolutePointToRobot(theRobotPose,  X, Y).translation.y()));
        }
    }
    state(alignToExactPosition) {
        transition{
            if (std::abs(AbsolutePointToRobot(theRobotPose,  4500.f, GOAL_Y).rotation) < 5_deg)
                goto searchForBall;
        }
        action{
            WalkToTarget(Pose2f(SPEED, SPEED, SPEED),
            Pose2f(AbsolutePointToRobot(theRobotPose,  4500.f, GOAL_Y).rotation, 0.f, 0.f));
        }
    }
    state(searchForBall) {
        transition{
            if (theLibCodeRelease.timeSinceBallWasSeen < 300)
                 goto supporter;
                //goto stand;
        }
        action{
            LookLeftAndRight();
        }
    }
    // state(stand) {
    //     transition {

    //     }
    //     action {
    //         Stand();
    //     }
    // }
}
#undef PI
#undef SPEED
#undef GOAL_Y
#undef X
#undef Y
