#define COMMONSPEED  0.3f
#define TURNSPEED  1.047f
#define GOAL_Y 600.f

option(Defender) {
    initial_state(launch) {
        transition {
            //deng ce shi le isValue
            if (theLibCodeRelease.timeSinceBallWasSeen > 2500 && theTeamBallModel.isValid==false)
                goto searchForBall;
            int area = GetAreaNumber(theTeamBallModel, theRobotPose);
            if (area ==  1)                 //  球在对方球场，盯着球
                goto focusBall;
            else if (area == 2 )            //  我方球场较远处    zhuanxiangqiu
                goto turnToBall;
            else if (area == 3)         //我方球场，较近处
                goto turnToBall;
        }
        action {
            //goto menkoude dingduan.
            LookForward();
            //  Annotation("I am in initial_state");
            //Pose2f Ball2Field = BallModel2Field(theBallModel,theRobotPose);
            //***  maybe can show the canshu **
            //int area = GetAreaNumber(theBallModel,theRobotPose);
            //Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.0, 0.0);
            Stand();

            //ShowTeamBallPose(theTeamBallModel);
            //ShowBallToRobot(AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()));
            //ShowRobotPose(theRobotPose);
        }
    }
    state(focusBall) {
        transition{
            if (theTeamBallModel.position.x() < -500.f)
                goto turnToBall;
            if (theLibCodeRelease.timeSinceBallWasSeen > 2000.f && theTeamBallModel.isValid==false)
                goto searchForBall;
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > -500.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > -500.f )
                goto goToRightArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > -500.f )
                goto goToMiddleArea;
        }
        action{
            LookForward();
            Stand();
        }
    }
    state(focusBall_1) {
        transition{
            if (theTeamBallModel.position.x() < -500.f)
                goto turnToBall;
            if (theLibCodeRelease.timeSinceBallWasSeen > 2000.f && theTeamBallModel.isValid==false)
                goto searchForBall;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > -500.f )
                goto goToRightArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > -500.f )
                goto goToMiddleArea;
        }
        action{
            LookForward();
            Stand();
        }
    }
    state(focusBall_2) {
        transition{
            if (theTeamBallModel.position.x() < -500.f)
                goto turnToBall;
            if (theLibCodeRelease.timeSinceBallWasSeen > 2000.f && theTeamBallModel.isValid==false)
                goto searchForBall;
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > -500.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > -500.f )
                goto goToRightArea;
        }
        action{
            LookForward();
            Stand();
        }
    }
    state(focusBall_3) {
        transition{
            if (theTeamBallModel.position.x() < -500.f)
                goto turnToBall;
            if (theLibCodeRelease.timeSinceBallWasSeen > 2000.f && theTeamBallModel.isValid==false)
                goto searchForBall;
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > -500.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > -500.f )
                goto goToMiddleArea;
        }
        action{
            LookForward();
            Stand();
        }
    }
    state(turnToBall) {
        transition {
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > -500.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > -500.f )
                goto goToRightArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > -500.f )
                goto goToMiddleArea;
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && theTeamBallModel.isValid==false)
                goto searchForBall;
            if (std::abs(theBallModel.estimate.position.angle()) < 5_deg)
                goto walkToBall;
        }
        action {
            //theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
        }
    }
    state(walkToBall) {
        transition {
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToRightArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > 0.f )
                goto goToMiddleArea;
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && theTeamBallModel.isValid==false)
                goto searchForBall;
            if (theBallModel.estimate.position.norm() < 500.f)
                goto alignToGoal_1;
            //goto alignBehindBall;
        }
        action {
            //theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED),
            theBallModel.estimate.position);
        }
    }
    state(alignToGoal_1) {
        transition {
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToRightArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > 0.f )
                goto goToMiddleArea;
            //  printf("%lf\n", AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation );
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && theTeamBallModel.isValid==false)
                goto searchForBall;
            if (theRobotPose.translation.y() > 0)
                goto alignToGoal_21;
            else
                goto alignToGoal_22;

        }
        action {

            //theHeadControlMode = HeadControl::focusBall;
            Stand();
        }
    }
    state(alignToGoal_21) {
        transition {
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToRightArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > 0.f )
                goto goToMiddleArea;
            //  printf("%lf\n", AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation );
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && theTeamBallModel.isValid==false)
                goto searchForBall;
            if (std::abs(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation) < 10_deg
            && std::abs(theBallModel.estimate.position.y()) < 100.f)
                goto alignBehindBall_1;


        }
        action {
            //printf("1\n");
            //theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED),
            Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation,
            theBallModel.estimate.position.x() - 400.f,
            theBallModel.estimate.position.y()));
        }
    }
    state(alignToGoal_22) {
        transition {
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToRightArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > 0.f )
                goto goToMiddleArea;
            //  printf("%lf\n", AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation );
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && theTeamBallModel.isValid==false)
                goto searchForBall;
            if (std::abs(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation) < 10_deg
            && std::abs(theBallModel.estimate.position.y()) < 100.f)
                goto alignBehindBall_1;


        }
        action {
            //printf("1\n");
            //theHeadControlMode = HeadControl::focusBall;
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED),
            Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation,
            theBallModel.estimate.position.x() - 400.f,
            theBallModel.estimate.position.y()));
        }
    }
    state(alignBehindBall_1) {
        transition {
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToRightArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > 0.f )
                goto goToMiddleArea;
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && theTeamBallModel.isValid==false)
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

            //theHeadControlMode = HeadControl::focusBall;
            Stand();
        }
    }
    state(alignBehindBall_21) {
        transition {
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToRightArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > 0.f )
                goto goToMiddleArea;
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && theTeamBallModel.isValid==false)
                goto searchForBall;

            if (theLibCodeRelease.between(theBallModel.estimate.position.y(), 30.f, 50.f)
            && theLibCodeRelease.between(theBallModel.estimate.position.x(), 190.f, 220.f)
            && std::abs(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation) < 2_deg)
                goto Choose;

        }
        action {

            //theHeadControlMode = HeadControl::focusBall;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED),
            Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, GOAL_Y).rotation,
            theBallModel.estimate.position.x() - 200.f,//150
            theBallModel.estimate.position.y() - 30.f));

        }
    }
    state(alignBehindBall_22) {
        transition {
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToRightArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > 0.f )
                goto goToMiddleArea;
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && theTeamBallModel.isValid==false)
                goto searchForBall;

            if (theLibCodeRelease.between(theBallModel.estimate.position.y(), 30.f, 50.f)
            && theLibCodeRelease.between(theBallModel.estimate.position.x(), 190.f, 220.f)
            && std::abs(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation) < 2_deg)
                goto Choose;

        }
        action {

            //theHeadControlMode = HeadControl::focusBall;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED),
            Pose2f(AbsolutePointToRobot(theRobotPose, 4500.f, -GOAL_Y).rotation,
            theBallModel.estimate.position.x() - 200.f,//150
            theBallModel.estimate.position.y() - 30.f));

        }
    }
    state(Choose) {
        transition{
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToRightArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > 0.f )
                goto goToMiddleArea;
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && theTeamBallModel.isValid==false)
                goto searchForBall;

            if (IfOppoInFront(theObstacleModel, theRobotPose) == 1)
                goto KickToRightSupporter;

            if (IfOppoInFront(theObstacleModel, theRobotPose) == 2)
                goto KickToLeftSupporter_1;

            if (IfOppoInFront(theObstacleModel, theRobotPose) == 0)
                goto Kick;

        }
        action{
            //theHeadControlMode = HeadControl::focusBall;
            Stand();
        }
    }
    state(Kick) {
        transition {
            if (theBallModel.estimate.position.norm() > 500)
                goto turnToBall;
            if (state_time > 10000 || (state_time > 1750 && action_done)) {
                if (theTeamBallModel.position.x() < -500.f)
                    goto turnToBall;
                if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > -500.f )
                    goto goToLeftArea;
                if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > -500.f )
                    goto goToRightArea;
                if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > -500.f )
                    goto goToMiddleArea;
            }
        }
        action {
            FocusBall();

            Kick(KickRequest::BQ_Kick, true);

        }
    }
    state(KickToRightSupporter) {
        transition{
            if (theBallModel.estimate.position.norm() > 500)
                goto turnToBall;
            if (state_time > 10000 || (state_time > 2000 && action_done)) {
                if (theTeamBallModel.position.x() < -500.f)
                    goto turnToBall;
                if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > -500.f )
                    goto goToLeftArea;
                if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > -500.f )
                    goto goToRightArea;
                if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > -500.f )
                    goto goToMiddleArea;
            }
        }
        action{
            FocusBall();
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
            if (theBallModel.estimate.position.norm() > 500)
                goto turnToBall;
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > 0.f )
                goto goToRightArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > 0.f )
                goto goToMiddleArea;

            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && theTeamBallModel.isValid==false)
                goto searchForBall;

            if (theBallModel.estimate.position.y() < -10.f && theBallModel.estimate.position.y() > -50.f
            && theBallModel.estimate.position.x() < 220.f && theBallModel.estimate.position.x() > 190.f)
                goto KickToLeftSupporter_2;

        }
        action {
            FocusBall();
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED),
            Pose2f(0, theBallModel.estimate.position.x() - 200.f,
            theBallModel.estimate.position.y() + 30.f));
        }
    }
    state(KickToLeftSupporter_2) {
        transition {
            if (state_time > 10000 || (state_time > 2000 && action_done)) {
                if (theTeamBallModel.position.x() < -500.f)
                    goto turnToBall;
                if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > -500.f )
                    goto goToLeftArea;
                if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > -500.f )
                    goto goToRightArea;
                if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > -500.f )
                    goto goToMiddleArea;
            }
        }
        action {
            FocusBall();
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
    // state(Standd)
    // {
    //     action{
    //         Stand();
    //     }
    // }
    state(searchForBall) {
        transition{
            // if(theTeamBallModel.isValid==true)
            //         goto Standd;
            if (theLibCodeRelease.timeSinceBallWasSeen < 300.0 || theTeamBallModel.isValid==true) {
                // if(theTeamBallModel.isValid==true)
                //     goto Standd;
                if (theTeamBallModel.position.x() < -500.f)
                    goto turnToBall;
                if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > -500.f )
                    goto goToLeftArea;
                if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > -500.f )
                    goto goToRightArea;
                if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > -500.f )
                    goto goToMiddleArea;
            }
        }
        action {
            // theHeadControlMode = HeadControl::LookForward;
            // if (theRobotPose.rotation >= 0) {
            //     SearchForBallLeft();
            // } else SearchForBallRight();
            SearchForBall(theRobotInfo, TURNSPEED, COMMONSPEED);
        }
    }
    state(goToRightArea) {
        transition{
            if (theLibCodeRelease.between( theRobotPose.translation.x() , -2700.0 , -2300.0 )
            && theLibCodeRelease.between(  theRobotPose.translation.y() , -2200.0, -1800.0 )
            && std::fabs(theRobotPose.rotation) < 4_deg )
                goto focusBall_3;
            if (theTeamBallModel.position.x() < -500.f)
                goto turnToBall;
            if (theLibCodeRelease.timeSinceBallWasSeen > 2000.f && theTeamBallModel.isValid==false)
                goto searchForBall;
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > -500.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > -500.f )
                goto goToRightArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > -500.f )
                goto goToMiddleArea;

        }
        action{
            LookForward();
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -2500.0, -2000.0);
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED), relatePoint );
        }
    }
    state(goToMiddleArea) {
        transition{
            if (theLibCodeRelease.between( theRobotPose.translation.x() , -2700.0 , -2300.0 )
            && theLibCodeRelease.between(  theRobotPose.translation.y() , -200.0, 200.0 )
            && std::fabs(theRobotPose.rotation) < 4_deg )
                goto focusBall_2;
            if (theTeamBallModel.position.x() < -500.f)
                goto turnToBall;
            if (theLibCodeRelease.timeSinceBallWasSeen > 2000.f && theTeamBallModel.isValid==false)
                goto searchForBall;
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > -500.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > -500.f )
                goto goToRightArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > -500.f )
                goto goToMiddleArea;
        }
        action{
            LookForward();
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -2500.0, 0.0);
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED), relatePoint );
        }
    }
    state(goToLeftArea) {
        transition{

            if (theLibCodeRelease.between( theRobotPose.translation.x() , -2700.0 , -2300.0 )
            && theLibCodeRelease.between(  theRobotPose.translation.y() , 1800.0, 2200.0 )
            && std::fabs(theRobotPose.rotation) < 4_deg )
                goto focusBall_1;
            if (theTeamBallModel.position.x() < -500.f)
                goto turnToBall;
            if (theLibCodeRelease.timeSinceBallWasSeen > 2000.f && theTeamBallModel.isValid==false)
                goto searchForBall;
            if (theTeamBallModel.position.y() >=  1000.0 && theTeamBallModel.position.x() > -500.f )
                goto goToLeftArea;
            if (theTeamBallModel.position.y() <= -1000.0 && theTeamBallModel.position.x() > -500.f )
                goto goToRightArea;
            if (theTeamBallModel.position.y() < 1000.0 && -1000.0 < theTeamBallModel.position.y() && theTeamBallModel.position.x() > -500.f )
                goto goToMiddleArea;
        }
        action{
            LookForward();
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -2500.0, 2000.0);
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED), relatePoint);
        }
    }
}
#undef COMMONSPEED
#undef TURNSPEED