/*
@author Xuanzhi Wang
the strategy of shoot the goal is so easy
according to the position of the keeper ,choose the left post or the right post.
quite well
*/
option(Penalty_shoot_4){
	unsigned int i=0;
    //unsigned int no=0;
    initial_state(start) {
        transition {
            if (state_time > 3000)
                goto shoot_striker_turnToBall;
        }
        action {
            theHeadControlMode = HeadControl::lookLeftAndRight;
            Stand();
        }
    }

    state(shoot_striker_turnToBall) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto shoot_striker_searchForBall;
            if (std::abs(theBallModel.estimate.position.angle()) < 5_deg)
                goto shoot_striker_walkToBall;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            WalkToTarget(Pose2f(0.2f, 0.2f, 0.2f),
            Pose2f(theBallModel.estimate.position.angle(),
            0.f,
            0.f));
        }
    }

    state(shoot_striker_walkToBall) {
       
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto shoot_striker_searchForBall;
            //if (theBallModel.estimate.position.x() < 200.f&&std::abs(theBallModel.estimate.position.y())<100.0f)
            if (state_time>2000&&theBallModel.estimate.position.norm() < 500.f)
            {
                
                if(i==1)
                {
            	   goto shoot_striker_alignToGoal_right;
                    i=0;
                }
                if(i==0)
                {
                    goto shoot_striker_alignToGoal_left;
                    i=1;
                }
            }
        }
        action {
        	
            theHeadControlMode = HeadControl::lookForward;
            WalkToTarget(Pose2f(0.2f, 0.2f, 0.2f),
            theBallModel.estimate.position);
        }
    }

    state(shoot_striker_alignToGoal_right) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto shoot_striker_searchForBall;
            //if (std::abs(theLibCodeRelease.angleToGoal) < 14_deg &&std::abs(theLibCodeRelease.angleToGoal) > 12_deg&& std::abs(theBallModel.estimate.position.y()) < 100.f)
            if(state_time>5000.f)
                goto shoot_striker_alignBehindBall_right;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            WalkToTarget(Pose2f(0.2f, 0.2f, 0.2f),
            Pose2f( -12_deg,
            theBallModel.estimate.position.x() - 270.f,
            theBallModel.estimate.position.y()));
        }
    }
    state(shoot_striker_alignToGoal_left) {
        transition {
            
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto shoot_striker_searchForBall;
            //if (std::abs(theLibCodeRelease.angleToGoal) <14_deg &&std::abs(theLibCodeRelease.angleToGoal) >12_deg&& std::abs(theBallModel.estimate.position.y()) < 100.f)
            if(state_time>5000.f)
                goto shoot_striker_alignBehindBall_left;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            WalkToTarget(Pose2f(0.2f, 0.2f, 0.2f),
            Pose2f( 12_deg,
            theBallModel.estimate.position.x() - 270.f,
            theBallModel.estimate.position.y()));
        }
    }

    state(shoot_striker_alignBehindBall_right) {
        transition {
            
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto shoot_striker_searchForBall;
            if ((theLibCodeRelease.between(theBallModel.estimate.position.y(), 20.f, 50.f)
            && theLibCodeRelease.between(theBallModel.estimate.position.x(), 190.f, 220.f))
            //&&std::abs(theLibCodeRelease.angleToGoal) < 14_deg &&std::abs(theLibCodeRelease.angleToGoal) > 12_deg)
            ||state_time>3000.f)
                goto shoot_striker_kick_right;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(0.2f, 0.2f, 0.2f),
            Pose2f(-12_deg,
            theBallModel.estimate.position.x() - 200.f,//150
            theBallModel.estimate.position.y() - 30.f));
        }
    }
    state(shoot_striker_alignBehindBall_left) {
        transition {
            
            if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto shoot_striker_searchForBall;
            if ((theLibCodeRelease.between(theBallModel.estimate.position.y(), -50.0f, -20.0f)
            && theLibCodeRelease.between(theBallModel.estimate.position.x(), 190.f, 220.f))
            //&&std::abs(theLibCodeRelease.angleToGoal) <14_deg &&std::abs(theLibCodeRelease.angleToGoal) >12_deg)
            || state_time>3000.f)
                goto shoot_striker_kick_left;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(0.2f, 0.2f, 0.2f),
            Pose2f(12_deg,
            theBallModel.estimate.position.x() - 200.f,//150
            theBallModel.estimate.position.y() + 30.f));
        }
    }

    state(shoot_striker_kick_right) {
        transition {
            if (state_time > 10000 || (state_time > 2000 && action_done))
                goto shoot_finished;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            Kick(KickRequest::BQ_Kick, true);
        }
    }

    state(shoot_striker_kick_left) {
        transition {
            if (state_time > 10000 || (state_time > 2000 && action_done))
                goto shoot_finished;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            Kick(KickRequest::BQ_Kick, false);
        }
    }

    state(shoot_striker_searchForBall) {
        transition {
            if (theLibCodeRelease.timeSinceBallWasSeen < 300)
                goto shoot_striker_turnToBall;
        }
        action {
            theHeadControlMode = HeadControl::lookLeftAndRight;
            //WalkAtRelativeSpeed(Pose2f(1.f, 0.f, 0.f));
        }
    }
    state(shoot_finished){
    	transition{
    		if(state_time>1000)
    			goto start;
    	}
        action{
            theHeadControlMode=HeadControl::lookForward;
            Stand();
        }
    }

}