/** Handle penalty state (and the actions to be performed after leaving the penalty state).
 *   This option is one level higher than the main game state handling as penalties
 *   might occur in most game states. */
option(HandlePenaltyState) 
{
    /** By default, the robot is not penalized and plays soccer according to the current game state.
    The chestbutton has to be pushed AND released to manually penalize a robot */
    float keeper_position_x = -4200.0f;
    float keeper_position_y = 0.0f;
    float striker_position_x=0.0f;
    float striker_position_y=0.0f;
    float other_position_x=-2500.0f;
    float other_position_y=0.f;
    float x,y;
    initial_state(notPenalized) {
        transition {
            if (theRobotInfo.penalty != PENALTY_NONE)
                goto penalized;
        }
        action {
            HandleGameState();
        }
    }

    /** In case of any penalty, the robots stands still. */
    state(penalized) {
        transition {
            if (theRobotInfo.penalty == PENALTY_NONE)
                goto preUnpenalize;
        }
        action {
            PlaySound("penalized.wav");
            theHeadControlMode = HeadControl::lookForward;
            SpecialAction(SpecialActionRequest::standHigh);
        }
    }

    state(preUnpenalize) {
        transition {
            if (theRobotInfo.penalty != PENALTY_NONE)
                goto penalized;
            //if (theRobotInfo.penalty == PENALTY_NONE)
               // goto backToField;
            else if (theGameInfo.state == STATE_INITIAL || state_time > 500)
                //goto backToField_1;
                goto notPenalized;
        }
        action {
            PlaySound("notPenalized.wav");
            theHeadControlMode = HeadControl::lookForward;
            Stand();
        }
    }
    /*state(backToField_1){
        transition{
            if (theRobotInfo.penalty != PENALTY_NONE)
                goto penalized;
            //if (theRobotInfo.penalty == PENALTY_NONE)
               // goto backToField;
            else if (theGameInfo.state == STATE_INITIAL || state_time > 1500)
                goto backToField_2;
        }
        action{
            WalkToTarget(Pose2f(0.5f,0.5f,0.5f),Pose2f(1.57f,0.0f,0.0f));
        }
    }
    state(backToField_2){
        transition{
            if (theRobotInfo.penalty != PENALTY_NONE)
                goto penalized;
            //if (theRobotInfo.penalty == PENALTY_NONE)
               // goto backToField;
            else if (theGameInfo.state == STATE_INITIAL || state_time > 1500)
                goto backToField_3;
        }
        action{
            WalkToTarget(Pose2f(0.5f,0.5f,0.5f),Pose2f(-1.57f,0.0f,0.0f));
        }
    }
    state(backToField_3){
        transition{
            if (theRobotInfo.penalty != PENALTY_NONE)
                goto penalized;
            //if (theRobotInfo.penalty == PENALTY_NONE)
               // goto backToField;
            else if (theGameInfo.state == STATE_INITIAL || state_time > 1500)
                goto backToField;
        }
        action{
            WalkToTarget(Pose2f(0.5f,0.5f,0.5f),Pose2f(-1.57f,0.0f,0.0f));
        }
    }
    state(backToField)
    {
    transition
    {
      if (theRobotInfo.number == 1
        || theRobotInfo.number == 2)
        goto strategy_striker;
    if (theRobotInfo.number == 3
        || theRobotInfo.number == 4)
        goto strategy_other;
      if (theRobotInfo.number == 5)
        goto goToKeeperPosition;
    }
    action
    {
       LookLeftAndRight();
       Stand();
    }
  }

  state(goToKeeperPosition)
  {
    transition
    {
      if (state_time>20000)
        goto notPenalized;
    } 
    action
    {
      x=keeper_position_x;
      y=keeper_position_y;
      Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, x, y);
      WalkToTarget(Pose2f(0.5f,0.5f,0.5f),relatePoint);
    }
  }
  state(strategy_striker)
  {
    transition
    {
        if(state_time>20000)
            goto notPenalized;
        if(theLibCodeRelease.timeSinceBallWasSeen < 300)
            goto notPenalized;
    }

    action
    {
        //TurnHead(0.5,30.0);
        //WalkToTarget(Pose2f(0.5f,0.5f,0.5f),Pose2f(theBallModel.estimate.position.angle(),theBallModel.estimate.position.x(),theBallModel.estimate.position.y()));
        //WalkToTarget(Pose2f(0.5f,0.5f,0.5f),Pose2f(0.0f,3000.0f,0.0f));
        x=striker_position_x;
        y=striker_position_y;
        Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, x, y);
        WalkToTarget(Pose2f(0.5f,0.5f,0.5f),relatePoint);
    }
  }
  state(strategy_other)
  {
    transition
    {
        if(state_time>20000)
            goto notPenalized;
        if(theLibCodeRelease.timeSinceBallWasSeen < 300)
            goto notPenalized;
    }

    action
    {
        //TurnHead(0.5,30.0);
        //WalkToTarget(Pose2f(0.5f,0.5f,0.5f),Pose2f(theBallModel.estimate.position.angle(),theBallModel.estimate.position.x(),theBallModel.estimate.position.y()));
        //WalkToTarget(Pose2f(0.5f,0.5f,0.5f),Pose2f(0.0f,3000.0f,0.0f));
        x=other_position_x;
        y=other_position_y;
        Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, x, y);
        WalkToTarget(Pose2f(0.5f,0.5f,0.5f),relatePoint);
    }
  }*/
  
}
