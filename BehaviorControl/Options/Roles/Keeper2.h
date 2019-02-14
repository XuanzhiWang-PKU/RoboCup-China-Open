#include <cstdio>
option(Keeper2) {
#define keeper_speed 0.5f
#define keeper_position -4200.f
#define fangshouqianbianjie -3500.f
//     initial_state(sitDown) {
//         transition {
//         }
//         action {
//             SpecialAction(SpecialActionRequest::keeperAction , false ) ;
//             theHeadControlMode = HeadControl::lookForward;
//         }
//     }
// }
    initial_state(launch)
    {
        transition{
            if (theLibCodeRelease.timeSinceBallWasSeen > 2500)
                goto searchForBall;
            if(theTeamBallModel.isValid==true&&
                theTeamBallModel.position.x()<fangshouqianbianjie&&
                std::fabs(theTeamBallModel.position.y())<1100.f
                &&theTeamBallModel.velocity.x()<-200.f)
                goto sitDown;
            if(std::fabs(theRobotPose.translation.x()+4200)>200)
                goto backToCenter;
            int area = GetAreaNumber(theTeamBallModel, theRobotPose);
            if (area == 1)                 //  球在对方球场，盯着球
                goto inPosiFocusBall;
            if (area == 2 )            //  我方球场较远处    zhuanxiangqiu
                goto turnToBall;
            if (area == 3)         //我方球场，较近处
                goto BallIsComing;
            if(state_time>3000)
                goto quiet;
        }
        action{
            Stand();
            theHeadControlMode = HeadControl::lookForward;
        }
    }

    state(backToCenter)
    {
        transition
        {
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            if (std::fabs(theRobotPose.translation.x() + 4200) < 200 && std::fabs(theRobotPose.translation.y()) < 100)
                goto launch;
        }
        action {
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.f, 0.f);
            WalkToTarget(Pose2f(keeper_speed, keeper_speed, keeper_speed), Pose2f(0.0f, relatePoint.translation.x(), relatePoint.translation.y()));
        }
    }

    state(inPosiFocusBall)
    {
        transition
        {
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            //if(theRobotPose.translation.x()<-4300&&theBallModel.estimate.position.norm()<400.f)
            if(theTeamBallModel.isValid==true&&
                theTeamBallModel.position.x()<fangshouqianbianjie &&
                std::fabs(theTeamBallModel.position.y())<1100.f
                &&theTeamBallModel.velocity.x()<-200.f)
                goto sitDown;
            int area = GetAreaNumber(theTeamBallModel, theRobotPose);
            if (area == 2 )            //  我方球场较远处    zhuanxiangqiu
                goto turnToBall;
            if (area == 3)         //我方球场，较近处
                goto BallIsComing;
            if(state_time>3000)
                goto quiet;
        }
        action {
            InPosiFocusBall();
        }
    }

    state(turnToBall)
    {
        transition
        {
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            //if(theRobotPose.translation.x()<-4300&&theBallModel.estimate.position.norm()<400.f)
            if(theTeamBallModel.isValid==true&&
                theTeamBallModel.position.x()<fangshouqianbianjie &&
                std::fabs(theTeamBallModel.position.y())<1100.f
                &&theTeamBallModel.velocity.x()<-200.f)
                goto sitDown;
            if(theTeamBallModel.position.y()>500.f)
                goto turnToBall_left;
            else if(theTeamBallModel.position.y()<-500.f)
                goto turnToBall_right;
            else goto turnToBall_center;
            int area = GetAreaNumber(theTeamBallModel, theRobotPose);
            if (area == 1)                 //  球在对方球场，盯着球
                goto inPosiFocusBall;
            if (area == 3)         //我方球场，较近处
                goto BallIsComing;
        }

        action
        {
            Stand();
            theHeadControlMode=HeadControl::lookLeftAndRight;
        }
    }
    state(turnToBall_center)
    {
        transition
        {
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            if(theTeamBallModel.position.y()>500.f)
                goto turnToBall_left;
            else if(theTeamBallModel.position.y()<-500.f)
                goto turnToBall_right;
            //if(theRobotPose.translation.x()<-4300&&theBallModel.estimate.position.norm()<400.f)
            if(theTeamBallModel.isValid==true&&
                theTeamBallModel.position.x()<fangshouqianbianjie &&
                std::fabs(theTeamBallModel.position.y())<1100.f
                &&theTeamBallModel.velocity.x()<-200.f)
                goto sitDown;
            int area = GetAreaNumber(theTeamBallModel, theRobotPose);
            if (area == 1)                 //  球在对方球场，盯着球
                goto inPosiFocusBall;
            if (area == 3)         //我方球场，较近处
                goto BallIsComing;
            if(state_time>3000)
                goto quiet;
        }
        action
        {
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.0, 0.0);
            WalkToTarget(Pose2f(keeper_speed,keeper_speed,keeper_speed),Pose2f(theBallModel.estimate.position.angle(),relatePoint.translation.x(),relatePoint.translation.y()));
        }
    }
    state(turnToBall_left)
    {
        transition
        {
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            //if(theRobotPose.translation.x()<-4300&&theBallModel.estimate.position.norm()<400.f)
            if(theTeamBallModel.isValid==true&&
                theTeamBallModel.position.x()<fangshouqianbianjie &&
                std::fabs(theTeamBallModel.position.y())<1100.f
                &&theTeamBallModel.velocity.x()<-200.f)
                goto sitDown;
            if(theTeamBallModel.position.y()<-500.f)
                goto turnToBall_right;
            else if(std::fabs(theTeamBallModel.position.y())<500.f)
                goto turnToBall_center;
            int area = GetAreaNumber(theTeamBallModel, theRobotPose);
            if (area == 1)                 //  球在对方球场，盯着球
                goto inPosiFocusBall;
            if (area == 3)         //我方球场，较近处
                goto BallIsComing;
            if(state_time>3000)
                goto quiet;
        }
        action
        {
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.0, 500.0);
            WalkToTarget(Pose2f(keeper_speed,keeper_speed,keeper_speed),Pose2f(theBallModel.estimate.position.angle(),relatePoint.translation.x(),relatePoint.translation.y()));
        }
    }
    state(turnToBall_right)
    {
        transition
        {
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            //if(theRobotPose.translation.x()<-4300&&theBallModel.estimate.position.norm()<400.f)
            if(theTeamBallModel.isValid==true&&
                theTeamBallModel.position.x()<fangshouqianbianjie &&
                std::fabs(theTeamBallModel.position.y())<1100.f
                &&theTeamBallModel.velocity.x()<-200.f)
                goto sitDown;
            if(theTeamBallModel.position.y()>500.f)
                goto turnToBall_left;
            else if(std::fabs(theTeamBallModel.position.y())<500.f)
                goto turnToBall_center;
            int area = GetAreaNumber(theTeamBallModel, theRobotPose);
            if (area == 1)                 //  球在对方球场，盯着球
                goto inPosiFocusBall;
            if (area == 3)         //我方球场，较近处
                goto BallIsComing;
            if(state_time>3000)
                goto quiet;
        }
        action
        {
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.0, -500.0);
            WalkToTarget(Pose2f(keeper_speed,keeper_speed,keeper_speed),Pose2f(theBallModel.estimate.position.angle(),relatePoint.translation.x(),relatePoint.translation.y()));
        }
    }
    state(BallIsComing)
    {
        transition
        {
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            //if(theRobotPose.translation.x()<-4300&&theBallModel.estimate.position.norm()<400.f)
            if(theTeamBallModel.isValid==true&&
                theTeamBallModel.position.x()<fangshouqianbianjie &&
                std::fabs(theTeamBallModel.position.y())<1100.f
                &&theTeamBallModel.velocity.x()<-200.f)
                goto sitDown;
            if(theBallModel.estimate.position.norm()<600.f)
                goto goToBall;
            int area = GetAreaNumber(theTeamBallModel, theRobotPose);
            if (area == 1)                 //  球在对方球场，盯着球
                goto inPosiFocusBall;
            if (area == 2 )            //  我方球场较远处    zhuanxiangqiu
                goto turnToBall;
        }
        action
        {
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.0, theTeamBallModel.position.y());
            WalkToTarget(Pose2f(keeper_speed,keeper_speed,keeper_speed),Pose2f(theBallModel.estimate.position.angle(),0.f,relatePoint.translation.y()));
        }
    }
    state(goToBall)
    {
        transition
        {
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            //if(theRobotPose.translation.x()<-4300&&theBallModel.estimate.position.norm()<400.f)
            if(theTeamBallModel.isValid==true&&
                theTeamBallModel.position.x()<fangshouqianbianjie &&
                std::fabs(theTeamBallModel.position.y())<1100.f
                &&theTeamBallModel.velocity.x()<-200.f)
                goto sitDown;
            //if(theBallModel.estimate.position.norm()<200.f)
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -50.f, -30.f)
            && theLibCodeRelease.between(theBallModel.estimate.position.x(), -210.f, -180.f))
                goto kickOut;
            int area = GetAreaNumber(theTeamBallModel, theRobotPose);
            if (area == 1)                 //  球在对方球场，盯着球
                goto inPosiFocusBall;
            if (area == 2 )            //  我方球场较远处    zhuanxiangqiu
                goto turnToBall;
        }
        action
        {
            WalkToTarget(Pose2f(keeper_speed,keeper_speed,keeper_speed),Pose2f(theBallModel.estimate.position.angle(),theBallModel.estimate.position.x(),theBallModel.estimate.position.y()-30.f));
        }
    }
    state(kickOut)
    {
        transition {
            if(theBallModel.estimate.position.norm()>200.f)
                goto launch;
            if (theLibCodeRelease.timeSinceBallWasSeen > 2500)
                goto searchForBall;
            if (state_time > 10000 || (state_time > 1750 && action_done))
                goto launch;
        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            Kick(KickRequest::BQ_Kick, true);

        }
    }
    state(sitDown) {
        transition {
            if (action_done && state_time > 4000)
                goto getUp;
        }
        action {
            SpecialAction(SpecialActionRequest::keeperAction , false ) ;
            theHeadControlMode = HeadControl::lookForward;
        }
    }
    state(getUp) {
        transition {
            if (action_done && state_time > 2000)
                goto launch ;
        }
        action {
            SpecialAction(SpecialActionRequest::newnewaction , false ) ;
            //GetUp();
        }
    }
    state(quiet)
    {
        transition
        {
            if (libCodeReleathetheLibCodeReleasese.timeSinceBallWasSeen > 2500)
                goto searchForBall;
            //if(theRobotPose.translation.x()<-4300&&theBallModel.estimate.position.norm()<400.f)
            if(theTeamBallModel.isValid==true&&
                theTeamBallModel.position.x()<fangshouqianbianjie &&
                std::fabs(theTeamBallModel.position.y())<1100.f
                &&theTeamBallModel.velocity.x()<-200.f)
                goto sitDown;
            if(std::fabs(theRobotPose.translation.x()+4200)>200)
                goto backToCenter;
            int area = GetAreaNumber(theTeamBallModel, theRobotPose);
            if (area == 1)                 //  球在对方球场，盯着球
                goto inPosiFocusBall;
            if (area == 2 )            //  我方球场较远处    zhuanxiangqiu
                goto turnToBall;
            if (area == 3)         //我方球场，较近处
                goto BallIsComing;
        }
        action
        {
            Stand();
            theHeadControlMode=HeadControl::lookLeftAndRight;
        }
        action {
            InPosiFocusBall();
        }
    }
    state(searchForBall) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen < 500)  {
                int area = GetAreaNumber(theTeamBallModel, theRobotPose);
                if (area == 1)                  //duifnagqiuchang    dingzheqiu
                    goto inPosiFocusBall;
                else if (area == 2 )            //wofang jiaoyuan    zhuanxiangqiu
                    goto    turnToBall;
                else if (area == 3 )            //wofang jiaojin    jinrupanduan
                    goto BallIsComing;
            }
            if(state_time>3000)
                goto quiet;

        }
        action {
            theHeadControlMode = HeadControl::lookLeftAndRight;
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4000.0, 0.0);
            WalkToTarget(Pose2f(keeper_speed, keeper_speed, keeper_speed), relatePoint);

        }
    }
}