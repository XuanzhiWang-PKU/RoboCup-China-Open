/** A test striker option without common decision */
#include <stdio.h>
#define PI 3.1415926
#define SPEED 1.0f
#define GOAL_Y 600.f

//using namespace std;
option(Striker) {
    initial_state(start) {
        transition {
            if (state_time > 1000)
                goto turnToBall;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            Stand();
        }
    }
    // initial_state(start) {
    //     transition {
    //         if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
    //             goto searchForBall;
    //         if (theGameInfo.kickOffTeam == theOwnTeamInfo.teamNumber && state_time > 2000)
    //             goto  turnToBall;
    //         else goto guard;
    //     }
    //     action {
    //         theHeadControlMode = HeadControl::focusBall;
    //         Stand();
    //     }
    // }
    // state(guard) {
    //     transition{
    //         if (state_time > 10000 ||
    //         theBallModel.estimate.velocity.x() != 0.0 ||
    //         theBallModel.estimate.velocity.y() != 0.0  )
    //             goto turnToBall;
    //     }
    //     action{
    //         theHeadControlMode = HeadControl::focusBall;
    //         Stand();
    //     }
    // }
    state(turnToBall) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if (std::abs(theBallModel.estimate.position.angle()) < 5_deg)
                goto walkToBall;
        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(SPEED, SPEED, SPEED),
            Pose2f(theBallModel.estimate.position.angle(),
            0.f,
            0.f));
        }
    }
    state(walkToBall) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if (theBallModel.estimate.position.norm() < 500.f)
                goto alignToGoal_1;
            //goto alignBehindBall;
        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(SPEED, SPEED, SPEED),
            theBallModel.estimate.position);
        }
    }
    state(alignToGoal_1) {
        transition {
            //  printf("%lf\n", AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation );
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if (theRobotPose.translation.y() > 0)
                goto alignToGoal_21;
            else
                goto alignToGoal_22;

        }
        action {

            theHeadControlMode = HeadControl::focusBall;
            Stand();
        }
    }
    state(alignToGoal_21) {
        transition {
            //  printf("%lf\n", AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation );
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if (std::abs(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation) < 10_deg
            && std::abs(theBallModel.estimate.position.y()) < 100.f)
                goto alignBehindBall_1;


        }
        action {
            //printf("1\n");
            theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(SPEED, SPEED, SPEED),
            Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation,
            theBallModel.estimate.position.x() - 400.f,
            theBallModel.estimate.position.y()));
        }
    }
    state(alignToGoal_22) {
        transition {
            //  printf("%lf\n", AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation );
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if (std::abs(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation) < 10_deg
            && std::abs(theBallModel.estimate.position.y()) < 100.f)
                goto alignBehindBall_1;


        }
        action {
            //printf("1\n");
            theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(SPEED, SPEED, SPEED),
            Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation,
            theBallModel.estimate.position.x() - 400.f,
            theBallModel.estimate.position.y()));
        }
    }
    state(alignBehindBall_1) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if (theRobotPose.translation.y() > 0) {
                //printf("1\n");
                goto alignBehindBall_21;
            } else{
                //printf("2\n");
                goto alignBehindBall_22;
            }
        }
        action {

            theHeadControlMode = HeadControl::focusBall;
            Stand();
        }
    }
    state(alignBehindBall_21) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (theLibCodeRelease.between(theBallModel.estimate.position.y(), 30.f, 50.f)
            && theLibCodeRelease.between(theBallModel.estimate.position.x(), 190.f, 220.f)
            && std::abs(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation) < 2_deg)
                goto Choose;

        }
        action {

            theHeadControlMode = HeadControl::focusBall;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(SPEED, SPEED, SPEED),
            Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation,
            theBallModel.estimate.position.x() - 200.f,//150
            theBallModel.estimate.position.y() - 30.f));

        }
    }
    state(alignBehindBall_22) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (theLibCodeRelease.between(theBallModel.estimate.position.y(), 30.f, 50.f)
            && theLibCodeRelease.between(theBallModel.estimate.position.x(), 190.f, 220.f)
            && std::abs(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation) < 2_deg)
                goto Choose;

        }
        action {

            theHeadControlMode = HeadControl::focusBall;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(SPEED, SPEED, SPEED),
            Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation,
            theBallModel.estimate.position.x() - 200.f,//150
            theBallModel.estimate.position.y() - 30.f));

        }
    }
    state(Choose) {
        transition{
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (IfOppoInFront(theObstacleModel, theRobotPose) == 1)
                goto KickToRightSupporter;

            if (IfOppoInFront(theObstacleModel, theRobotPose) == 2)
                goto KickToLeftSupporter_1;

            if (IfOppoInFront(theObstacleModel, theRobotPose) == 0)
                goto Kick;

        }
        action{
            theHeadControlMode = HeadControl::focusBall;
            Stand();
        }
    }
    state(Kick) {
        transition {
            if (state_time > 10000 || (state_time > 2000 && action_done))
                goto turnToBall;
        }
        action {
            theHeadControlMode = HeadControl::focusBall;

            Kick(KickRequest::BQ_Kick, true);

        }
    }
    state(KickToRightSupporter) {
        transition{
            if (state_time > 10000 || (state_time > 2000 && action_done))
                goto turnToBall;
        }
        action{
            theHeadControlMode = HeadControl::focusBall;
            if (theRobotPose.translation.y() > 0)
                InWalkKick(WalkKickVariant(WalkKicks::sidewardsInner, Legs::left),
                Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation,
                theBallModel.estimate.position.x() - 150.f,
                theBallModel.estimate.position.y() - 45.f));
            else
                InWalkKick(WalkKickVariant(WalkKicks::sidewardsInner, Legs::left),
                Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation,
                theBallModel.estimate.position.x() - 150.f,
                theBallModel.estimate.position.y() - 45.f));
        }
    }
    state(KickToLeftSupporter_1) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (theBallModel.estimate.position.y() < -10.f && theBallModel.estimate.position.y() > -50.f
            && theBallModel.estimate.position.x() < 220.f && theBallModel.estimate.position.x() > 190.f)
                goto KickToLeftSupporter_2;

        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(SPEED, SPEED, SPEED),
            Pose2f(0, theBallModel.estimate.position.x() - 200.f,
            theBallModel.estimate.position.y() + 30.f));
        }
    }
    state(KickToLeftSupporter_2) {
        transition {
            if (state_time > 10000 || (state_time > 2000 && action_done))
                goto turnToBall;
        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            if (theRobotPose.translation.y() > 0)
                InWalkKick(WalkKickVariant(WalkKicks::sidewardsInner, Legs::right),
                Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation,
                theBallModel.estimate.position.x() - 150.f,
                theBallModel.estimate.position.y() + 55.f));
            else
                InWalkKick(WalkKickVariant(WalkKicks::sidewardsInner, Legs::right),
                Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation,
                theBallModel.estimate.position.x() - 150.f,
                theBallModel.estimate.position.y() + 55.f));
        }
    }
    state(searchForBall) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen < 300)
                goto turnToBall;
        }
        action {
            // theHeadControlMode = HeadControl::lookForward;
            // if (theRobotPose.rotation >= 0) {
            //     SearchForBallLeft();
            // } else SearchForBallRight();
            SearchForBall(theRobotInfo, SPEED);
        }
    }
}
