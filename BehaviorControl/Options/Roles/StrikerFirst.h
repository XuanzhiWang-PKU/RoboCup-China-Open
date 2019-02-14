/**
 * @file StrikerFirst.h
 *
 * If we start the game, firstly, striker kick the ball to the exact position(X, Y) to the
 * supporter2.When this part done, goto the nomal Striker.
 *
 * @author Xuanzhi Wang
 */

#define COMMONSPEED 0.4f
#define TURNSPEED 1.047f
#define GOAL_Y 600.f
#define X 1500.f
#define Y -1500.f
#define BQ_X (-170.f)
#define BQ_Y (-45.f)
option(StrikerFirst) {
    initial_state(start) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            if (theGameInfo.kickOffTeam == theOwnTeamInfo.teamNumber)
                goto  turnToBallFirst;
            else goto guard;
        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            Stand();
        }
    }
    // initial_state(start) {
    //     transition {
    //             goto striker;
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
                goto striker;

        }
        action{
            theHeadControlMode = HeadControl::focusBall;
            Stand();
        }
    }
    state(striker) {
        transition{}
        action{
            Striker(TURNSPEED, COMMONSPEED);
        }
    }
    state(turnToBallFirst) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if (std::abs(theBallModel.estimate.position.angle()) < 5_deg)
                goto walkToBallFirst;
        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED),
            Pose2f(theBallModel.estimate.position.angle(),
            0.f,
            0.f));
        }
    }
    state(walkToBallFirst) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if (theBallModel.estimate.position.norm() < 250.f)
                goto alignToPosition;

        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED),
            theBallModel.estimate.position);
        }
    }
    state(alignToPosition) {
        transition {
            //  printf("%lf\n", AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation );
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if (std::abs(AbsolutePointToRobot(theRobotPose, X, Y).rotation) < 10_deg
            && std::abs(theBallModel.estimate.position.y()) < 100.f)
                goto alignBehindBallFirst;


        }
        action {

            theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED),
            Pose2f(AbsolutePointToRobot(theRobotPose,  X, Y).rotation,
            theBallModel.estimate.position.x() - 250.f,
            theBallModel.estimate.position.y()));
        }
    }
    state(alignBehindBallFirst) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;

            if (theLibCodeRelease.between(theBallModel.estimate.position.y(), (-BQ_Y - 10.f), (-BQ_Y + 20.f))
            && theLibCodeRelease.between(theBallModel.estimate.position.x(), (-BQ_X - 10.f), (-BQ_X + 20.f))
            && std::abs(AbsolutePointToRobot(theRobotPose,  X, Y).rotation) < 2_deg)
                goto jugde;

        }
        action {

            theHeadControlMode = HeadControl::focusBall;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED),
            Pose2f(AbsolutePointToRobot(theRobotPose,  X, Y).rotation,
            theBallModel.estimate.position.x() + BQ_X,//150
            theBallModel.estimate.position.y() + BQ_Y));

        }
    }
    state(jugde) {
        transition{
            unsigned i = 0;
            if (option_time > 10000)
                goto Kick;
            else{
                for (i = 0; i < theTeamData.teammates.size(); i++) {
                    if (theTeamData.teammates[i].number == 2 &&
                    std::abs(theTeamData.teammates[i].theRobotPose.translation.x() - X) < 50.f &&
                    std::abs(theTeamData.teammates[i].theRobotPose.translation.y() - Y) < 50.f)
                        goto Kick;
                }

            }
        }
        action{
            Stand();
            theHeadControlMode = HeadControl::focusBall;
        }
    }
    state(Kick) {
        transition {
            if (state_time > 10000 || (state_time > 1750 && action_done))
                goto striker;
        }
        action {
            theHeadControlMode = HeadControl::focusBall;

            Kick(KickRequest::BQ_Kick, true);

        }
    }
    state(searchForBall) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen < 300)
                goto turnToBallFirst;
            // goto stand;
        }
        action {
            // theHeadControlMode = HeadControl::lookForward;
            // if (theRobotPose.rotation >= 0) {
            //     SearchForBallLeft();
            // } else SearchForBallRight();
            SearchForBall(theRobotInfo, TURNSPEED, COMMONSPEED);
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
#undef COMMONSPEED
#undef TURNSPEED
#undef PI
#undef GOAL_Y
#undef X
#undef Y