/**
 * @file Supporter.h
 *
 * When the Supporter could see the ball by itself, Supporter would kick without common decision.
 * When the Supporter couldn't see the ball by itself, will goto SearchForBall,which base on the communication.
 *
 * Detials:Devide the ground into two parts--1:left 2:right.
 *         Devide the kick action into three parts to avoid obstacle--Kick, KickToRightSupporter, KickToLeftSupporter 
 * @author Xuanzhi Wang
 */
#include <stdio.h>

#define SPEED 0.3f
#define GOAL_Y 600.f
#define X 1500.f
#define Y -1500.f
#define BQ_X (-200.f)
#define BQ_Y (-30.f)
#define LEFT_X (-160.f)
#define LEFT_Y (-50.f)
#define RIGHT_X (-160.f)
#define RIGHT_Y (50.f)
//using namespace std;
option(Supporter) {
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
    state(guard) {
        transition{
            if (state_time > 10000 ||
            theBallModel.estimate.velocity.x() != 0.0 ||
            theBallModel.estimate.velocity.y() != 0.0  )
                goto turnToBall;
        }
        action{
            theHeadControlMode = HeadControl::focusBall;
            Stand();
        }
    }

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
            WalkToTargetInPathMode(Pose2f(SPEED, SPEED, SPEED),
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

            if (theLibCodeRelease.between(theBallModel.estimate.position.y(), -BQ_Y - 10.f, -BQ_Y + 20.f)
            && theLibCodeRelease.between(theBallModel.estimate.position.x(), -BQ_X - 10.f, -BQ_X + 20.f)
            && std::abs(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation) < 2_deg)
                goto choose;

        }
        action {

            theHeadControlMode = HeadControl::focusBall;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(SPEED, SPEED, SPEED),
            Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation,
            theBallModel.estimate.position.x() + BQ_X,//150
            theBallModel.estimate.position.y() + BQ_Y));

        }
    }
    state(alignBehindBall_22) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (theLibCodeRelease.between(theBallModel.estimate.position.y(), -BQ_Y - 10.f, -BQ_Y + 20.f)
            && theLibCodeRelease.between(theBallModel.estimate.position.x(), -BQ_X - 10.f, -BQ_X + 20.f)
            && std::abs(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation) < 2_deg)
                goto choose;

        }
        action {

            theHeadControlMode = HeadControl::focusBall;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(SPEED, SPEED, SPEED),
            Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation,
            theBallModel.estimate.position.x() + BQ_X,//150
            theBallModel.estimate.position.y() + BQ_Y));

        }
    }
    state(choose) {
        transition{
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (IfOppoInFront(theObstacleModel, theRobotPose) == 1)
                goto KickToRightSupporter_1;

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
    
     state(KickToRightSupporter_1) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (theBallModel.estimate.position.y() < (-RIGHT_Y + 20.f) && theBallModel.estimate.position.y() > (-RIGHT_Y - 10.f)
            && theBallModel.estimate.position.x() < (-RIGHT_X + 20.f) && theBallModel.estimate.position.x() > (-RIGHT_X - 10.f))
                goto KickToRightSupporter_2;

        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(SPEED, SPEED, SPEED),
            Pose2f(0, theBallModel.estimate.position.x() + RIGHT_X,
            theBallModel.estimate.position.y() + RIGHT_Y));
        }
    }
    state(KickToRightSupporter_2) {
        transition {
            if (state_time > 10000 || (state_time > 2000 && action_done))
                goto turnToBall;
        }
        action {
            theHeadControlMode = HeadControl::focusBall;
           if (theRobotPose.translation.y() > 0)
                InWalkKick(WalkKickVariant(WalkKicks::turnOut, Legs::left),
                Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation,
                theBallModel.estimate.position.x() + RIGHT_X,
                theBallModel.estimate.position.y() + RIGHT_Y));
            else
                InWalkKick(WalkKickVariant(WalkKicks::turnOut, Legs::left),
                Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation,
                theBallModel.estimate.position.x() + RIGHT_X,
                theBallModel.estimate.position.y() + RIGHT_Y));
        }
    }
    state(KickToLeftSupporter_1) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (theBallModel.estimate.position.y() < (-LEFT_Y + 10.f) && theBallModel.estimate.position.y() > (-LEFT_Y - 20.f)
            && theBallModel.estimate.position.x() < (-LEFT_X + 20.f) && theBallModel.estimate.position.x() > (-LEFT_X - 10.f))
                goto KickToLeftSupporter_2;

        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(SPEED, SPEED, SPEED),
            Pose2f(0, theBallModel.estimate.position.x() + LEFT_X,
            theBallModel.estimate.position.y() + LEFT_Y));
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
                InWalkKick(WalkKickVariant(WalkKicks::turnOut, Legs::right),
                Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation,
                theBallModel.estimate.position.x() + LEFT_X,
                theBallModel.estimate.position.y() + LEFT_Y));
            else
                InWalkKick(WalkKickVariant(WalkKicks::turnOut, Legs::right),
                Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation,
                theBallModel.estimate.position.x() + LEFT_X,
                theBallModel.estimate.position.y() + LEFT_Y));
        }
    }
    state(searchForBall) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen < 300)
                goto turnToBall;
               //goto stand;
        }
        action {
            SearchForBall(theRobotInfo, SPEED);
        }
    }
    // state(stand){
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
#undef BQ_X (-200.f)
#undef BQ_Y (-30.f)
#undef LEFT_X (-160.f)
#undef LEFT_Y (-50.f)
#undef RIGHT_X (-160.f)
#undef RIGHT_Y (50.f)