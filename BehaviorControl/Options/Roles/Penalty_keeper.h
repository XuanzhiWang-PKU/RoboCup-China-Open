/*
@author Xuanzhi Wang
the strategy of the keeper is easy too.
firstly, when the ball is 1000mm away, the robot try his best to reach that side(left or right)
then, if the ball is closer,the robot carry his keeperAction immediately.
*/
option(Penalty_keeper){
    initial_state(shoot_keeper) {
        transition{
            if(state_time>2500)
                goto launch;
        }
        action{
            theHeadControlMode=HeadControl::lookLeftAndRight;
            Stand();
        }
    }
    state(launch)
    {

        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > 2500)
                goto searchForBall;
            if(theBallModel.estimate.position.x()<1500) 
                goto BallIsComing;  
        }
    
        action
        {
            theHeadControlMode = HeadControl::lookForward;
            Stand();
        }
    }

    state(BallIsComing)     
    {
        transition{
            if(theLibCodeRelease.timeSinceBallWasSeen > 3000.0)
                    goto searchForBall;
            if(theBallModel.estimate.position.x() < 1000.0  )
                goto sitDown;
        }
        action{
            if(theBallModel.estimate.position.y()>0)
                {WalkToTarget(Pose2f(0.1f,0.1f,0.8f),Pose2f(0.0f,0.0f,100.0f));
                PlaySound("forwardLeft.wav");}
            else 
                {WalkToTarget(Pose2f(0.1f,0.1f,0.8f),Pose2f(0.0f,0.0f,-100.0f));
                PlaySound("forwardRight.wav");}
        }
    }

    state(sitDown)
    {
        transition
        {
            if(action_done && state_time > 4000)
                goto getUp;
        }
        action
        {
            //PlaySound("falling.wav");
            SpecialAction(SpecialActionRequest::keeperAction , false ) ;
            //theHeadControlMode = HeadControl::lookForward;
        }
    }
    
    state(getUp)
    {
        transition
        {
            if(action_done && state_time > 2000)
                goto shoot_finished;
        }
        action
        {
            
            SpecialAction(SpecialActionRequest::keeperStandUp , false ) ;
            theHeadControlMode=HeadControl::lookForward;
            //Stand(); 
        }
    }

    state(searchForBall)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen < 500)  
            {
                goto BallIsComing; 
            }
        }
        action
        {
            theHeadControlMode = HeadControl::lookForward;
            Stand();
        }
    }
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

    state(shoot_finished){
    	transition{
    		if(state_time>1000)
    			goto shoot_keeper;
    	}
        action{
            theHeadControlMode=HeadControl::lookForward;
            Stand();
        }
    }
}