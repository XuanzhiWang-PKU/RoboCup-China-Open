/**	
 * To keeper：当球在对方球场时，keeper位于初始位置，  (-4300.0,0.0),	并focusball; 
 * @author Xuanzhi Wang
 */
option(InPosiFocusBall,(float)(0.5f)SPEED)
{
	initial_state(launch)
	{
		transition{
			int area = GetAreaNumber(theTeamBallModel,theRobotPose);
			if(area !=  1)	//DANG QIU BUZAI DUIFANGQYU TUICHU!
				goto finish  ;   
			if(theLibCodeRelease.timeSinceBallWasSeen > 1500)
				goto finish;

			if( (theRobotPose.translation.y()  > 10.0 ) || (theRobotPose.translation.y() < -10.0 ) )
				goto WalkToForbiddenCenter;
			else goto finish;
		}
		action{
			Stand();
			theHeadControlMode = HeadControl::focusBall;
		}
					
	}

	state(WalkToForbiddenCenter)
	{
		transition{
			if(theLibCodeRelease.between(theRobotPose.translation.y(),-10.0,10.0) )
				goto finish;
		}
		action{
			theHeadControlMode = HeadControl::focusBall;
			Pose2f relatePoint = AbsolutePointToRobot(theRobotPose,-4200.0,0.0);
			WalkToTarget(Pose2f(SPEED,SPEED,SPEED),relatePoint);
		}
	}

	target_state(finish)
	{
		transition{
			if(state_time > 2000)
				goto launch;
		}
		action
		{
			Stand();
			theHeadControlMode = HeadControl::focusBall;
		}
	}
}
