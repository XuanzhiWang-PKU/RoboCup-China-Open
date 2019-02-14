#include <cstdio>
option(Keeper_simplify) {
#define keeper_speed 0.2f
#define keeper_position -4200.f
#define keeperField 1500.f
    initial_state(launch)
    {
        transition{

            if(theBallModel.estimate.velocity.x()<-600.f&&theBallModel.estimate.position.x()<keeperField)
                goto sitDown;
            if(std::fabs(theRobotPose.translation.x()+4200)>200)
                goto backToCenter;
            if(theLibCodeRelease.timeSinceBallWasSeen>2000.f)
                goto searchForBall;
            if(theBallModel.estimate.position.norm()<keeperField)
                goto turnToBall;
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
            if(theBallModel.estimate.velocity.x()<-600.f&&theBallModel.estimate.position.x()<keeperField)
                goto sitDown;
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            if (std::fabs(theRobotPose.translation.x() + 4200) < 200 && std::fabs(theRobotPose.translation.y()) < 100 && std::fabs(theLibCodeRelease.angleToGoal)<10_deg)
                goto launch;
            if(theBallModel.estimate.position.norm()<keeperField)
                goto turnToBall;
        }
        action {
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, -4200.f, 0.f);
            WalkToTarget(Pose2f(keeper_speed, keeper_speed, keeper_speed), Pose2f(theLibCodeRelease.angleToGoal, relatePoint.translation.x(), relatePoint.translation.y()));
        }
    }

    

    state(turnToBall)
    {
        transition
        {
            if(theBallModel.estimate.velocity.x()<-600.f&&theBallModel.estimate.position.x()<keeperField)
                goto sitDown;
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            if(std::fabs(theBallModel.estimate.position.angle())<5_deg)
                goto BallIsComing;
            if(theBallModel.estimate.position.norm()>keeperField)
                goto launch;
            
        }

        action
        {
            WalkToTarget(Pose2f(keeper_speed,keeper_speed,keeper_speed),Pose2f(theBallModel.estimate.position.angle(),0.f,0.f));
            theHeadControlMode=HeadControl::lookForward;
        }
    }
    
    state(BallIsComing)
    {
        transition
        {
            if(theBallModel.estimate.velocity.x()<-600.f&&theBallModel.estimate.position.x()<keeperField)
                goto sitDown;
            if(state_time>6000.f)
                goto goToBall;
            //if(theLibCodeRelease.timeSinceBallWasSeen>1400.f)//||theBallModel.estimate.velocity.x()<-600.f)
            
            if(theBallModel.estimate.velocity.x()>500||theBallModel.estimate.position.x()>keeperField)
                goto launch;
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
        }
        action
        {
            WalkToTarget(Pose2f(keeper_speed,keeper_speed,keeper_speed),Pose2f(theBallModel.estimate.position.angle(),theBallModel.estimate.position.x(),theBallModel.estimate.position.y()));
        }
    }
    state(goToBall)
    {
        transition
        {
            if(theBallModel.estimate.velocity.x()<-600.f&&theBallModel.estimate.position.x()<keeperField)
                goto sitDown;
            if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
                goto searchForBall;
            //if(theRobotPose.translation.x()<-4300&&theBallModel.estimate.position.norm()<400.f)
            // if(theTeamBallModel.position.x()<-4200&&std::fabs(theTeamBallModel.position.y())<1100.f)
            //     goto sitDown;
            if(theBallModel.estimate.position.norm()<200.f)
                goto kickOut;
            
        }
        action
        {
            WalkToTarget(Pose2f(keeper_speed,keeper_speed,keeper_speed),Pose2f(theBallModel.estimate.position.angle(),theBallModel.estimate.position.x(),theBallModel.estimate.position.y()));
        }
    }
    state(kickOut)
    {
        transition {
            
            if (theLibCodeRelease.timeSinceBallWasSeen > 2500)
                goto searchForBall;
            if (state_time > 10000 || (state_time > 1750 && action_done))
                goto launch;
        }
        action {
            theHeadControlMode = HeadControl::focusBall;
            Kick(KickRequest::BL_Kick, true);

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
            SpecialAction(SpecialActionRequest::newAction);
        }
    }
    
    state(searchForBall) {
        transition 
        {
            if(theLibCodeRelease.timeSinceBallWasSeen<200.f&& theBallModel.estimate.position.norm()<1000.f)
                goto turnToBall;
            if(theLibCodeRelease.timeSinceBallWasSeen<200.f)
                goto launch;
        }
        action {
            theHeadControlMode = HeadControl::lookLeftAndRight;
        }
    }
}