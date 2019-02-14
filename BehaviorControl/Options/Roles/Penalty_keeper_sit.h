#define sit_speed 0.5f
option(Penalty_keeper_sit){
    initial_state(start)
    {
        transition{
            if(state_time>400.f)
                goto walkStraight;
        }
        action{
            theHeadControlMode=HeadControl::lookForward;
            Stand();
        }

    }
    state(walkStraight)
    {
        transition{
            if(state_time>4900.f||theRobotPose.translation.x()<-3900.f)
                goto sitDown;
        }
        action{
            WalkToTarget(Pose2f(sit_speed,sit_speed,sit_speed),Pose2f(0.f,1000.f,0.f));
        }
    }
    state(sitDown) {
        transition{
            if(state_time>10000.f)
                goto getUp;
        }
        action {
            SpecialAction(SpecialActionRequest::keeperAction , false );
            theHeadControlMode = HeadControl::lookForward;
        }
    }
    state(getUp) {
        transition {
            if (action_done && state_time > 2000)
                goto start;
        }
        action {
            GetUp();
        }
    }
}