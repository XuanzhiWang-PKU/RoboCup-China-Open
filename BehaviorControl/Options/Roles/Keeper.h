#include <cstdio>
#define COMMONSPEED  0.3f
#define TURNSPEED  1.047f
option(Keeper) {
    initial_state(launch) {
        transition {
            //printf("launch\n");
            //deng ce shi le isValue

            Pose2f relate = AbsolutePointToRobot(theRobotPose, 0.f, 0.f);
            if (std::abs(relate.rotation) < 0.1f) {
                if (theLibCodeRelease.timeSinceBallWasSeen > 2500)
                    goto searchForBall;
                int area = GetAreaNumber(theTeamBallModel, theRobotPose);
                if (theTeamBallModel.velocity.x() < -300.0
                && std::abs(GetY(theRobotPose, theTeamBallModel.velocity) ) < 750
                && theBallModel.estimate.position.x() < -2000)
                    goto sitDown;
                if (area == 1)                 //  球在对方球场，盯着球
                    goto inPosiFocusBall;
                if (area == 2 )            //  我方球场较远处    zhuanxiangqiu
                    goto turnToBall;
                if (area == 3)         //我方球场，较近处
                    goto BallIsComing;
            }
        }
        action {
            //goto menkoude dingduan.
            theHeadControlMode = HeadControl::lookForward;
            //  Annotation("I am in initial_state");
            //Pose2f Ball2Field = BallModel2Field(theBallModel,theRobotPose);
            //***  maybe can show the canshu **
            //int area = GetAreaNumber(theBallModel,theRobotPose);
            // Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.0, 0.0);
            // WalkToTarget(Pose2f(0.2f, 0.1f, 0.1f), Pose2f(-theRobotPose.rotation, 0.0, 0.0));
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.0, 0.0);
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED), relatePoint);
            ShowTeamBallPose(theTeamBallModel);
            ShowBallToRobot(AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()));
            ShowRobotPose(theRobotPose);
        }
    }
    state(inPosiFocusBall) {
        transition {
            //printf("inPosiFocusBall\n");
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            if (theTeamBallModel.velocity.x() < -300.0
            && std::abs(GetY(theRobotPose, theTeamBallModel.velocity) ) < 750
            && theBallModel.estimate.position.x() < -2000)                goto sitDown;
            int area = GetAreaNumber(theTeamBallModel, theRobotPose);
            if (area == 2)          //  球在我方较远处   转向球
                goto turnToBall;
            else if (area == 3 )            //wofang jiaojin    jinrupanduan
                goto BallIsComing;
            if (action_done)
                goto focusBall;
        }
        action{

            InPosiFocusBall();

            ShowTeamBallPose(theTeamBallModel);
            ShowBallToRobot(AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()));
            ShowRobotPose(theRobotPose);
        }
    }
    state(focusBall) {
        transition{
            //printf("focusBall\n");
            if (theLibCodeRelease.timeSinceBallWasSeen > 2000)
                goto searchForBall;
            if (theTeamBallModel.velocity.x() < -300.0
            && std::abs(GetY(theRobotPose, theTeamBallModel.velocity) ) < 750
            && theBallModel.estimate.position.x() < -3000)                goto sitDown;
            int area = GetAreaNumber(theTeamBallModel, theRobotPose);
            if (area == 1 )                 //duifnagqiuchang    dingzheqiu
                goto inPosiFocusBall  ;
            else if (area == 2 )            //wofang jiaoyuan    zhuanxiangqiu
                goto turnToBall;
            else if (area == 3)         //wofang jiaojin    jinrupanduan
                goto BallIsComing   ;
        }
        action{
            theHeadControlMode = HeadControl::focusBall;
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.0, 0.0);
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED), relatePoint);
            ShowTeamBallPose(theTeamBallModel);
            ShowBallToRobot(AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()));
            ShowRobotPose(theRobotPose);
        }
    }
    state(turnToBall) {             ///   julijiaoyuan
        transition {
            //printf("turnToBall\n");
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            if (theTeamBallModel.velocity.x() < -300.0
            && std::abs(GetY(theRobotPose, theTeamBallModel.velocity) ) < 750
            && theBallModel.estimate.position.x() < -2000)                goto sitDown;
            int area = GetAreaNumber(theTeamBallModel, theRobotPose);
            if (area == 1 )                 //duifnagqiuchang    dingzheqiu
                goto inPosiFocusBall;
            else if (area == 2 )            //wofang jiaoyuan    zhuanxiangqiu
                goto turnToBall;
            else if (area == 3)         //wofang jiaojin    jinrupanduan
                goto BallIsComing;
        }
        action {
            float angle = AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()).rotation;
            if (std::fabs(angle) < 5_deg) {
                Stand();
                theHeadControlMode = HeadControl::lookForward;
            }
            if (std::fabs(angle ) < 10_deg )    {
                Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.0, 0.0);
                if ( theRobotPose.translation.y() > 150.f || theRobotPose.translation.y() < -150.f  )
                    WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED), relatePoint );
            } else if (angle > 10_deg)  {
                Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.0, 500.f);
                if (theRobotPose.translation.y() > 550.f || theRobotPose.translation.y() < 450.f)
                    WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED), relatePoint);
            } else {
                Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.0, -500.0);
                if (theRobotPose.translation.y()  < 550.f || theRobotPose.translation.y() > -450.f)
                    WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED), relatePoint);
            }

            ShowTeamBallPose(theTeamBallModel);
            ShowBallToRobot(AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()));
            ShowRobotPose(theRobotPose);
        }
    }
    state(BallIsComing) {   // 有问题！！   当求在球门附近的时候，直接坐下？ 防止对方射门球速过快来不及防守。
        transition{
            //printf("BallIsComing\n");
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000.0)
                goto searchForBall;
            if (theTeamBallModel.velocity.x() < -300.0
            && std::abs(GetY(theRobotPose, theTeamBallModel.velocity) ) < 750
            && theBallModel.estimate.position.x() < -2000)                goto sitDown;

            if ( IfGoToBall(theBallModel, theObstacleModel) )   //关于 是否去将球踢开。～！！
                goto GoToBall;

            else goto sitDown;
        }
        action{
            if (theTeamBallModel.position.x() < -2800.f)
                WalkToTarget(Pose2f(30.f, 30.f, 30.f),
                Pose2f( AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()).rotation, 0.f, theTeamBallModel.position.y() ));
            else if (theTeamBallModel.position.x() > -2800.f)
                WalkToTarget(Pose2f(30.f, 30.f, 30.f),
                Pose2f( AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()).rotation, 0.f, 0.f));
            theHeadControlMode = HeadControl::focusBall;

            ShowTeamBallPose(theTeamBallModel);
            ShowBallToRobot(AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()));
            ShowRobotPose(theRobotPose);
        }
    }
    state(GoToBall) {
        transition{
            //printf("CoToBall\n");
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000.0)
                goto searchForBall;
            if (theTeamBallModel.velocity.x() < -300.0
            && std::abs(GetY(theRobotPose, theTeamBallModel.velocity) ) < 750
            && theBallModel.estimate.position.x() < -2000)                goto sitDown;
            if (theBallModel.estimate.position.norm() < 300.f)
                goto alignBehindBall;
        }
        action{
            theHeadControlMode = HeadControl::lookForward;
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED),
            //AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()));
            Pose2f(theBallModel.estimate.position.angle(), theBallModel.estimate.position.x() - 200.f, theBallModel.estimate.position.y() - 30.f));
            ShowTeamBallPose(theTeamBallModel);
            ShowBallToRobot(AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()));
            ShowRobotPose(theRobotPose);
        }
    }
    state(alignBehindBall) {
        transition {
            //printf("alignBehindBall\n");
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            if ( theBallModel.estimate.position.norm() < 200.f )
                //thetheLibCodeRelease.between(theBallModel.estimate.position.y(), 30.f, 50.f)
                //&& thetheLibCodeRelease.between(theBallModel.estimate.position.x(), 190.f, 220.f))
                goto kick;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED),
            //Pose2f(
            //-AbsolutePointToRobot(theRobotPose,theTeamBallModel.position.x() - 200.f, theTeamBallModel.position.y() - 30.f).rotation,

            //AbsolutePointToRobot(theRobotPose,theTeamBallModel.position.x(), theTeamBallModel.position.y()).translation.x()-200.f,
            //AbsolutePointToRobot(theRobotPose,theTeamBallModel.position.x(), theTeamBallModel.position.y()).translation.y()-30.f)

            Pose2f(theBallModel.estimate.position.angle(), theBallModel.estimate.position.x() - 200.f, theBallModel.estimate.position.y() - 30.f));
            //Pose2f(-theRobotPose.rotation,theRobotPose.translation.x()+theTeamBallModel.position.x() - 200.f, theRobotPose.translation.y()+theTeamBallModel.position.y() - 30.f)

            ShowTeamBallPose(theTeamBallModel);
            ShowBallToRobot(AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()));
            ShowRobotPose(theRobotPose);
        }
    }
    state(kick) {               //zhuanxiang duifangqiumen zai kick!
        transition{
            //printf("kick\n");
            if (action_done || state_time > 500) {
                int area = GetAreaNumber(theTeamBallModel, theRobotPose);
                if (theTeamBallModel.velocity.x() < -300.0
                && std::abs(GetY(theRobotPose, theTeamBallModel.velocity) ) < 750
                && theBallModel.estimate.position.x() < -2000)                    goto sitDown;
                if (area ==  1)                 //duifnagqiuchang    dingzheqiu
                    goto inPosiFocusBall;
                else if (area == 2 )            //wofang jiaoyuan    zhuanxiangqiu
                    goto    turnToBall;
                else if (area == 3)         //wofang jiaojin    jinrupanduan
                    goto BallIsComing;
                if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                    goto searchForBall;
            }
        }
        action{
            /*  InWalkKick(WalkRequest::left,
                    Pose2f(-1*theRobotPose.rotation,
                        theBallModel.estimate.position.x() - 160.f,
                    theBallModel.estimate.position.y() - 55.f));

            */
            theHeadControlMode = HeadControl::lookForward;

            Kick(KickRequest::BL_Kick, true);

            ShowTeamBallPose(theTeamBallModel);
            ShowBallToRobot(AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()));
            ShowRobotPose(theRobotPose);
        }
    }
    state(sitDown) {
        transition {
            //printf("sitDown\n");
            if (action_done && state_time > 4000)
                goto getUp;
        }
        action {
            //调用specialaction;

            SpecialAction(SpecialActionRequest::keeperAction , false ) ;
            theHeadControlMode = HeadControl::lookForward;

            ShowTeamBallPose(theTeamBallModel);
            ShowBallToRobot(AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()));
            ShowRobotPose(theRobotPose);
        }
    }
    state(getUp) {
        transition {
            //printf("getUp\n");
            if (action_done && state_time > 2000)
                goto launch ;
        }
        action {

            SpecialAction(SpecialActionRequest::keeperStandUp , false ) ;
        }
    }
    state(searchForBall_Turn) {
        transition {
            //printf("searchForBall_Turn\n");
            if (theLibCodeRelease.timeSinceBallWasSeen < 500)  {
                int area = GetAreaNumber(theTeamBallModel, theRobotPose);
                if (area == 1)                  //duifnagqiuchang    dingzheqiu
                    goto inPosiFocusBall  ;
                else if (area == 2 )            //wofang jiaoyuan    zhuanxiangqiu
                    goto turnToBall;
                else if (area == 3 )            //wofang jiaojin    jinrupanduan
                    goto BallIsComing   ;
            }
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4000.0, 0.0);
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED), relatePoint);
            ShowTeamBallPose(theTeamBallModel);
            ShowBallToRobot(AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()));
            ShowRobotPose(theRobotPose);
        }
    }
    state(searchForBall) {
        transition {
            //printf("searchForBall\n");
            if (theLibCodeRelease.timeSinceBallWasSeen > 5000)
                goto searchForBall_Turn;
            if (theLibCodeRelease.timeSinceBallWasSeen < 500)  {
                int area = GetAreaNumber(theTeamBallModel, theRobotPose);
                if (area == 1)                  //duifnagqiuchang    dingzheqiu
                    goto inPosiFocusBall  ;
                else if (area == 2 )            //wofang jiaoyuan    zhuanxiangqiu
                    goto    turnToBall;
                else if (area == 3 )            //wofang jiaojin    jinrupanduan
                    goto BallIsComing   ;
            }
        }
        action {
            theHeadControlMode = HeadControl::lookLeftAndRight;
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4000.0, 0.0);
            WalkToTarget(Pose2f(TURNSPEED, COMMONSPEED, COMMONSPEED), relatePoint);
            ShowTeamBallPose(theTeamBallModel);
            ShowBallToRobot(AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()));
            ShowRobotPose(theRobotPose);
        }
    }
}
#undef COMMONSPEED
#undef TURNSPEED