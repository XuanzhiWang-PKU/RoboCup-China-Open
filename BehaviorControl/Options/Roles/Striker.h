/** A test striker option with common decision */

/**
 * @file Striker.h
 *
 * When the Striker could see the ball by itself, Striker would kick without common decision.
 * When the Striker couldn't see the ball by itself, will goto SearchForBall,which base on the communication.
 *
 * Detials:*Devide the ground into two parts--1:left 2:right.
 *         *Devide the kick action into three parts to avoid obstacle--Kick, KickToRightSupporter, KickToLeftSupporter
 *         *If the opponent robot is within one meter while alian to goal, it's better for robot to just kick to avoid
 *          letting opponet robot get the opportunity.
 *         *As for walkToBall part, robot can judge how to achive it--pathMode or just walk to ball.What's more it can
 *          translate between the two mode flexiblely.
 *
 * @author Xuanzhi Wang
 */
#include <stdio.h>


#define GOAL_Y 600.f
#define X 1500.f
#define Y -1500.f
#define BQ_X (-170.f)
#define BQ_Y (-45.f)
#define LEFT_X (-150.f)
#define LEFT_Y (-50.f)
#define RIGHT_X (-150.f)
#define RIGHT_Y (50.f)
#define SPEED (0.5f)

//using namespace std;
option(Striker, (float)(1.047f) turnSpeed, (float)(0.3f) commonSpeed) {
    static unsigned int lastKickTime = 0;
    static bool firstKickInAState = true;
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
                goto chooseToWalkToBall;
        }
        action {

            firstKickInAState = true;
            theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed),
            Pose2f(theBallModel.estimate.position.angle(),
            0.f,
            0.f));
        }
    }
    state(chooseToWalkToBall) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if (theObstacleModel.obstacles.size() > 0) {
                unsigned minDisIdx = 0;
                for (unsigned i = 0; i < theObstacleModel.obstacles.size(); i++) {
                    if (theObstacleModel.obstacles[i].center.norm() < theObstacleModel.obstacles[minDisIdx].center.norm()) {
                        minDisIdx = i;
                    }
                }
                if (theObstacleModel.obstacles[minDisIdx].center.norm() < theBallModel.estimate.position.norm()) {
                    goto walkToBallInPathMode;
                } else goto walkToBall;
            } else goto walkToBall;


        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            Stand();
        }
    }
    state(walkToBallInPathMode) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if (theObstacleModel.obstacles.size() > 0) {
                unsigned minDisIdx = 0;
                for (unsigned i = 0; i < theObstacleModel.obstacles.size(); i++) {
                    if (theObstacleModel.obstacles[i].center.norm() < theObstacleModel.obstacles[minDisIdx].center.norm()) {
                        minDisIdx = i;
                    }
                }
                if (theObstacleModel.obstacles[minDisIdx].center.norm() > theBallModel.estimate.position.norm()) {
                    goto chooseToWalkToBall;
                }
            }
            if (theBallModel.estimate.position.norm() < 300.f)
                goto chooseToAlignToGoal;
            //goto alignBehindBall;
        }
        action {
            theHeadControlMode = HeadControl::focusBall;

            WalkToTargetInPathMode(Pose2f(turnSpeed, commonSpeed, commonSpeed),
            theBallModel.estimate.position);
        }
    }
    state(walkToBall) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if (theObstacleModel.obstacles.size() > 0) {
                unsigned minDisIdx = 0;
                for (unsigned i = 0; i < theObstacleModel.obstacles.size(); i++) {
                    if (theObstacleModel.obstacles[i].center.norm() < theObstacleModel.obstacles[minDisIdx].center.norm()) {
                        minDisIdx = i;
                    }
                }
                if (theObstacleModel.obstacles[minDisIdx].center.norm() < theBallModel.estimate.position.norm()) {
                    goto chooseToWalkToBall;
                }
            }

            if (theBallModel.estimate.position.norm() < 300.f)
                goto chooseToAlignToGoal;
            //goto alignBehindBall;
        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed),
            theBallModel.estimate.position);
        }
    }
    //fen wei liang ge jie duan 1:zai zuo ban chang chao zuo bian qiu men ti .
    state(chooseToAlignToGoal) {
        transition {
            //  printf("%lf\n", AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation );
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if (theRobotPose.translation.y() > 0)
                goto alignToLeftGoal;
            else
                goto alignToRightGoal;

        }
        action {

            theHeadControlMode = HeadControl::focusBall;
            Stand();
        }
    }
    state(alignToLeftGoal) {
        transition {
            //  printf("%lf\n", AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation );

            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (theObstacleModel.obstacles.size() > 0) {
                Obstacle theClosestObst = TheClosestObst(theObstacleModel);
                if (theClosestObst.center.norm() < 1000.f && std::abs(theClosestObst.center.y()) > 200 )
                    goto justBehindBallToKick;
                else if (theClosestObst.center.norm() < 1000.f && theClosestObst.center.y() > 0) {
                    goto justBehindBallToKickRight;
                } else if (theClosestObst.center.norm() < 1000.f && theClosestObst.center.y() < 0) {
                    goto justBehindBallToKickLeft;
                }
            }
            if (std::abs(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation) < 10_deg
                    && std::abs(theBallModel.estimate.position.y()) < 100.f)
                goto chooseToAlignBehindBall;


        }
        action {
            //printf("1\n");
            theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed),
            Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation,
            theBallModel.estimate.position.x() - 250.f,
            theBallModel.estimate.position.y()));
        }
    }
    state(alignToRightGoal) {
        transition {
            //  printf("%lf\n", AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation );
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if (theObstacleModel.obstacles.size() > 0) {
                Obstacle theClosestObst = TheClosestObst(theObstacleModel);
                if (theClosestObst.center.norm() < 1000.f && std::abs(theClosestObst.center.y()) > 200 )
                    goto justBehindBallToKick;
                else if (theClosestObst.center.norm() < 1000.f && theClosestObst.center.y() > 0) {
                    goto justBehindBallToKickRight;
                } else if (theClosestObst.center.norm() < 1000.f && theClosestObst.center.y() < 0) {
                    goto justBehindBallToKickLeft;
                }
            }

            if (std::abs(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation) < 10_deg
                    && std::abs(theBallModel.estimate.position.y()) < 100.f)
                goto chooseToAlignBehindBall;
        }
        action {
            //printf("1\n");
            theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed),
            Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation,
            theBallModel.estimate.position.x() - 250.f,
            theBallModel.estimate.position.y()));
        }
    }
    //
    state(chooseToAlignBehindBall) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if (theRobotPose.translation.y() > 0) {
                //printf("1\n");
                goto alignBehindBallToLeft;
            } else{
                //printf("2\n");
                goto alignBehindBallToRight;
            }
        }
        action {

            theHeadControlMode = HeadControl::focusBall;
            Stand();
        }
    }
    state(alignBehindBallToLeft) {
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
            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed),
            Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation,
            theBallModel.estimate.position.x() + BQ_X,//150
            theBallModel.estimate.position.y() + BQ_Y));

        }
    }
    state(alignBehindBallToRight) {
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
            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed),
            Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation,
            theBallModel.estimate.position.x() + BQ_X,//150
            theBallModel.estimate.position.y() + BQ_Y));

        }
    }
    state(justBehindBallToKick) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (theLibCodeRelease.between(theBallModel.estimate.position.y(), -BQ_Y - 10.f, -BQ_Y + 20.f)
            && theLibCodeRelease.between(theBallModel.estimate.position.x(), -BQ_X - 10.f, -BQ_X + 20.f))
                goto kick;

        }
        action {

            theHeadControlMode = HeadControl::focusBall;
            ShowBallPose(theBallModel);

            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed),
            Pose2f(0.f,
            theBallModel.estimate.position.x() + BQ_X,//150
            theBallModel.estimate.position.y() + BQ_Y));

        }
    }
    state(choose) {
        transition{
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (IfOppoInFront(theObstacleModel, theRobotPose) == 1)
                goto walkToKickPositionToRight;

            if (IfOppoInFront(theObstacleModel, theRobotPose) == 2)
                goto walkToKickPositionToLeft;

            if (IfOppoInFront(theObstacleModel, theRobotPose) == 0)
                goto kick;

        }
        action{
            theHeadControlMode = HeadControl::focusBall;
            Stand();
        }
    }
    state(kick) {
        transition {
            // if (option_time - lastKickTime < 2000) {
            //     goto turnToBall;
            // }
            if (theBallModel.estimate.position.norm() > 500)
                goto turnToBall;
            if (state_time > 10000 || (state_time > 1750 && action_done))
                goto turnToBall;
        }
        action {
            theHeadControlMode = HeadControl::focusBall;

            if (firstKickInAState) {
                firstKickInAState = false;
                lastKickTime = option_time;
            }
            Kick(KickRequest::BQ_Kick, true);

        }
    }
    state(justBehindBallToKickRight) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (theBallModel.estimate.position.y() < (-RIGHT_Y + 20.f) && theBallModel.estimate.position.y() > (-RIGHT_Y - 10.f)
            && theBallModel.estimate.position.x() < (-RIGHT_X + 20.f) && theBallModel.estimate.position.x() > (-RIGHT_X - 10.f))
                goto kickToRightSupporter;

        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed),
            Pose2f(0,
            theBallModel.estimate.position.x() + RIGHT_X,
            theBallModel.estimate.position.y() + RIGHT_Y));
        }
    }
    state(walkToKickPositionToRight) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (theBallModel.estimate.position.y() < (-RIGHT_Y + 20.f) && theBallModel.estimate.position.y() > (-RIGHT_Y - 10.f)
            && theBallModel.estimate.position.x() < (-RIGHT_X + 20.f) && theBallModel.estimate.position.x() > (-RIGHT_X - 10.f))
                goto kickToRightSupporter;

        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed),
            Pose2f(0, theBallModel.estimate.position.x() + RIGHT_X,
            theBallModel.estimate.position.y() + RIGHT_Y));
        }
    }
    state(kickToRightSupporter) {
        transition {
            if (state_time > 10000 || (state_time > 1750 && action_done))
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
    state(justBehindBallToKickLeft) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (theBallModel.estimate.position.y() < (-LEFT_Y + 10.f) && theBallModel.estimate.position.y() > (-LEFT_Y - 20.f)
            && theBallModel.estimate.position.x() < (-LEFT_X + 20.f) && theBallModel.estimate.position.x() > (-LEFT_X - 10.f))
                goto kickToLeftSupporter;

        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed),
            Pose2f(0,
            theBallModel.estimate.position.x() + LEFT_X,
            theBallModel.estimate.position.y() + LEFT_Y));
        }
    }
    state(walkToKickPositionToLeft) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (theBallModel.estimate.position.y() < (-LEFT_Y + 10.f) && theBallModel.estimate.position.y() > (-LEFT_Y - 20.f)
            && theBallModel.estimate.position.x() < (-LEFT_X + 20.f) && theBallModel.estimate.position.x() > (-LEFT_X - 10.f))
                goto kickToLeftSupporter;

        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed),
            Pose2f(0, theBallModel.estimate.position.x() + LEFT_X,
            theBallModel.estimate.position.y() + LEFT_Y));
        }
    }
    state(kickToLeftSupporter) {
        transition {
            if (state_time > 10000 || (state_time > 1750 && action_done))
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
            SearchForBall(theRobotInfo, turnSpeed, commonSpeed);
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
#undef GOAL_Y
#undef X
#undef Y
#undef BQ_X
#undef BQ_Y
#undef LEFT_X
#undef LEFT_Y
#undef RIGHT_X
#undef RIGHT_Y