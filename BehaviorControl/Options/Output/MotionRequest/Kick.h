option(Kick,((KickRequest) KickMotionID )id  ,(bool) (false) mirror )
{   
    initial_state(launch)
    {
        transition
        {
            if(theMotionInfo.motion == MotionRequest::kick &&
               theMotionInfo.kickRequest.kickMotionType == id&&
               theMotionInfo.kickRequest.mirror == mirror        )
               goto execute;
        }
        action
        {
            theMotionRequest.motion = MotionRequest::kick;
            theMotionRequest.kickRequest.kickMotionType = id;
            theMotionRequest.kickRequest.mirror = mirror;
        }
    }
    
    state(execute)
    {
        transition
        {
            if(theMotionRequest.kickRequest.kickMotionType == KickRequest::none)
                goto finished;
        }
        
        action
        {
            theMotionRequest.motion = MotionRequest::kick;
            theMotionRequest.kickRequest.kickMotionType = id;
            theMotionRequest.kickRequest.mirror = mirror;
            theMotionRequest.kickRequest.kickMotionType= KickRequest::none;
        }
    }   
    
    target_state(finished)
    {
        transition
        {
          if(theMotionInfo.motion != MotionRequest::kick ||
               theMotionInfo.kickRequest.kickMotionType != id ||
               theMotionInfo.kickRequest.mirror != mirror        )
                goto launch;
        }
        action
        {   
            
            theHeadControlMode = HeadControl::lookForward;
            Stand();
        }
    }
}
