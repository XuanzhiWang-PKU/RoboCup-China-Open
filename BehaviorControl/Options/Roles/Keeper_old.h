/**********************  守门员   *************

思路：keeper 不出禁区。
    在每个state里面都有一个判断，如果球速过快，则坐下！
    1、long distance : turn to ball;
    2.short distance :  dunxia   shoumen
    ****    待解决问题： 球在球场的位置！ ！***

    BallIsComing() ;   关于是否去踢球，根据现场再调整

    关于 防守时动作   根据XZT的动作
***********************************************/

//the keeper is used for test，
/*
主要检测：1、对于对方的大脚射门和近距离的正面射门能否有效的挡住，及当坐下防守起来的处理，
        2、对于球在禁区的处理。
        3、将对于球在某区域的检测改为球的远近的检测。
        4、如果球在较远的位置且范围在球门前方，则站在门口的中间。否则站在左右两侧
*/
#define SPEED 1.0f
option(Keeper) {
    initial_state(launch) {
        transition {
            if (thetheLibCodeRelease.timeSinceBallWasSeen > 2500)
                goto searchForBall;
            int area = GetAreaNumber(theTeamBallModel, theRobotPose);
            if (theTeamBallModel.velocity.x() < -300.0 && std::abs(GetY(theRobotPose, theTeamBallModel.velocity) ) < 750)
                goto sitDown;
            if (area ==  1)                 //  球在对方球场，盯着球
                goto inPosiFocusBall;
            else if (area == 2 )            //  我方球场较远处    zhuanxiangqiu
                goto turnToBall;
            else if (area == 3)         //我方球场，较近处
                goto BallIsComing;
        }
        action {
            //goto menkoude dingduan.
            theHeadControlMode = HeadControl::focusBall;
            //  Annotation("I am in initial_state");
            //Pose2f Ball2Field = BallModel2Field(theBallModel,theRobotPose);
            //***  maybe can show the canshu **
            //int area = GetAreaNumber(theBallModel,theRobotPose);
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.0, 0.0);
            Stand();



            ShowBallToField(theTeamBallModel.position);
            ShowBallToRobot(AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()));
            ShowRobotToField(theRobotPose);
        }
    }
    state(inPosiFocusBall) {
        transition {
            if (thetheLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            if (theTeamBallModel.velocity.x() < -300.0  )
                goto sitDown;
            int area = GetAreaNumber(theBallModel, theRobotPose);
            if (area == 2)          //  球在我方较远处   转向球
                goto    turnToBall;
            else if (area == 3 )            //wofang jiaojin    jinrupanduan
                goto BallIsComing   ;
            if (action_done)
                goto focusBall;
        }
        action{

            InPosiFocusBall(SPEED);

            Pose2f Ball2Field = BallModel2Field(theBallModel, theRobotPose);
            ShowBallToField(Ball2Field);
            ShowBallToRobot(theBallModel.estimate.position);
            ShowRobotToField(theRobotPose);
        }
    }
    state(focusBall) {
        transition{
            if (theLibCodeRelease.timeSinceBallWasSeen > 2000)
                goto searchForBall;
            if (theBallModel.estimate.velocity.x() < -300.0  )
                goto sitDown;
            int area = GetAreaNumber(theBallModel, theRobotPose);
            if (area == 1 )                 //duifnagqiuchang    dingzheqiu
                goto inPosiFocusBall  ;
            else if (area == 2 )            //wofang jiaoyuan    zhuanxiangqiu
                goto    turnToBall;
            else if (area == 3)         //wofang jiaojin    jinrupanduan
                goto BallIsComing   ;
        }
        action{
            theHeadControlMode = HeadControl::focusBall;
            Stand();


            Pose2f Ball2Field = BallModel2Field(theBallModel, theRobotPose);
            ShowBallToField(Ball2Field);
            ShowBallToRobot(theBallModel.estimate.position);
            ShowRobotToField(theRobotPose);
        }
    }
    // nao suizhe qiu zai butong d jiaodu zou dao san ge bu tong d dian
    state(turnToBall) {             ///   julijiaoyuan
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            if (theBallModel.estimate.velocity.x() < -300.0  )
                goto sitDown;
            int area = GetAreaNumber(theBallModel, theRobotPose);
            if (area == 1 )                 //duifnagqiuchang    dingzheqiu
                goto inPosiFocusBall  ;
            else if (area == 2 )            //wofang jiaoyuan    zhuanxiangqiu
                goto turnToBall;
            else if (area == 3)         //wofang jiaojin    jinrupanduan
                goto BallIsComing   ;
        }
        action {
            float angle = theBallModel.estimate.position.angle();
            if (std::fabs(angle ) < 10_deg )    {
                Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.0, 0.0);
                if ( theRobotPose.translation.y() > 150.f || theRobotPose.translation.y() < -150.f  )
                    WalkToTarget(Pose2f(20.f, 20.f, 20.f ), relatePoint );
            } else if (angle > 10_deg)  {
                Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.0, 500.f);
                if (theRobotPose.translation.y() > 550.f || theRobotPose.translation.y() < 450.f)
                    WalkToTarget(Pose2f(20.f, 20.f, 20.f), relatePoint);
            } else {
                Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.0, -500.0);
                if (theRobotPose.translation.y()  < 550.f || theRobotPose.translation.y() > -450.f)
                    WalkToTarget(Pose2f(20.f, 20.f, 20.f), relatePoint);
            }



            Pose2f Ball2Field = BallModel2Field(theBallModel, theRobotPose);
            ShowBallToField(Ball2Field);
            ShowBallToRobot(theBallModel.estimate.position);
            ShowRobotToField(theRobotPose);
        }
    }
    state(BallIsComing) {   // 有问题！！   当求在球门附近的时候，直接坐下？ 防止对方射门球速过快来不及防守。
        transition{
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000.0)
                goto searchForBall;
            if (theBallModel.estimate.velocity.x() < -300.0  )
                goto sitDown;

            if ( IfGoToBall(theBallModel, theObstacleModel) )   //关于 是否去将球踢开。～！！
                goto GoToBall;

            else goto sitDown;
        }
        action{
            WalkToTarget(Pose2f(30.f, 30.f, 30.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f ));
            theHeadControlMode = HeadControl::focusBall;

            Pose2f Ball2Field = BallModel2Field(theBallModel, theRobotPose);
            ShowBallToField(Ball2Field);
            ShowBallToRobot(theBallModel.estimate.position);
            ShowRobotToField(theRobotPose);
        }
    }
    state(GoToBall) {
        transition{
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000.0)
                goto searchForBall;
            if (theBallModel.estimate.velocity.x() < -300.0  )
                goto sitDown;
            if (theBallModel.estimate.position.norm() < 300.f)
                goto alignBehindBall;
        }
        action{
            theHeadControlMode = HeadControl::lookForward;
            WalkToTarget(Pose2f(20.f, 20.f, 20.f), Pose2f( theBallModel.estimate.position));

            Pose2f Ball2Field = BallModel2Field(theBallModel, theRobotPose);
            ShowBallToField(Ball2Field);
            ShowBallToRobot(theBallModel.estimate.position);
            ShowRobotToField(theRobotPose);
        }
    }
    state(alignBehindBall) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            if ( state_time > 3000 && std::abs(theRobotPose.rotation) < 5_deg)
                goto kick;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            WalkToTarget(Pose2f(20.f, 20.f, 20.f),
            Pose2f(-1 * theRobotPose.rotation  ,
            theBallModel.estimate.position.x() - 250.f, theBallModel.estimate.position.y() - 55.f));


            Pose2f Ball2Field = BallModel2Field(theBallModel, theRobotPose);
            ShowBallToField(Ball2Field);
            ShowBallToRobot(theBallModel.estimate.position);
            ShowRobotToField(theRobotPose);
        }
    }
    state(kick) {               //zhuanxiang duifangqiumen zai kick!
        transition{
            if (action_done) {
                if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                    goto searchForBall;
                int area = GetAreaNumber(theBallModel, theRobotPose);
                if (theBallModel.estimate.velocity.x() < -300.0  )
                    goto sitDown;
                if (area ==  1)                 //duifnagqiuchang    dingzheqiu
                    goto inPosiFocusBall;
                else if (area == 2 )            //wofang jiaoyuan    zhuanxiangqiu
                    goto    turnToBall;
                else if (area ==     4)         //wofang jiaojin    jinrupanduan
                    goto BallIsComing;
            }
        }
        action{
            /*  InWalkKick(WalkRequest::left,
                    Pose2f(-1*theRobotPose.rotation,
                        theBallModel.estimate.position.x() - 160.f,
                    theBallModel.estimate.position.y() - 55.f));

            */
            theHeadControlMode = HeadControl::lookForward;

            Kick(KickRequest::BL_Kick, false);

            Pose2f Ball2Field = BallModel2Field(theBallModel, theRobotPose);
            ShowBallToField(Ball2Field);
            ShowBallToRobot(theBallModel.estimate.position);
            ShowRobotToField(theRobotPose);
        }
    }
    state(sitDown) {
        transition {
            if (action_done && state_time > 4000)
                goto getUp;
        }
        action {
            //调用specialaction;

            SpecialAction(SpecialActionRequest::keeperAction , false ) ;
            theHeadControlMode = HeadControl::lookForward;

            Pose2f Ball2Field = BallModel2Field(theBallModel, theRobotPose);
            ShowBallToField(Ball2Field);
            ShowBallToRobot(theBallModel.estimate.position);
            ShowRobotToField(theRobotPose);
        }
    }
    state(getUp) {
        transition {
            if (action_done && state_time > 2000)
                goto launch ;
        }
        action {

            SpecialAction(SpecialActionRequest::keeperStandUp , false ) ;
        }
    }
    state(searchForBall_Turn) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen < 500)  {
                int area = GetAreaNumber(theBallModel, theRobotPose);
                if (area == 1)                  //duifnagqiuchang    dingzheqiu
                    goto inPosiFocusBall  ;
                else if (area == 2 )            //wofang jiaoyuan    zhuanxiangqiu
                    goto    turnToBall;
                else if (area == 4 )            //wofang jiaojin    jinrupanduan
                    goto BallIsComing   ;
            }
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            WalkToTarget(Pose2f(8.f, 8.f, 8.f),  Pose2f( 2.f, 1.f, 1.f) );

            Pose2f Ball2Field = BallModel2Field(theBallModel, theRobotPose);
            ShowBallToField(Ball2Field);
            ShowBallToRobot(theBallModel.estimate.position);
            ShowRobotToField(theRobotPose);

        }
    }
    state(searchForBall) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > 5000)
                goto searchForBall_Turn;
            if (theLibCodeRelease.timeSinceBallWasSeen < 500)  {
                int area = GetAreaNumber(theBallModel, theRobotPose);
                if (area == 1)                  //duifnagqiuchang    dingzheqiu
                    goto inPosiFocusBall  ;
                else if (area == 2 )            //wofang jiaoyuan    zhuanxiangqiu
                    goto    turnToBall;
                else if (area == 4 )            //wofang jiaojin    jinrupanduan
                    goto BallIsComing   ;
            }
        }
        action {
            theHeadControlMode = HeadControl::searchForBall;
            Stand();

            Pose2f Ball2Field = BallModel2Field(theBallModel, theRobotPose);
            ShowBallToField(Ball2Field);
            ShowBallToRobot(theBallModel.estimate.position);
            ShowRobotToField(theRobotPose);
        }
    }
}




