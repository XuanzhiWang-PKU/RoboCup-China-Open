option(Penalty_shoot_x) {
    initial_state(start) {
        transition{
            if (theLibCodeRelease.timeSinceBallWasSeen < 100) {
                if (theBallModel.estimate.position.y() > 100.f)
                    goto leftKick;
                else if (theBallModel.estimate.position.y() < -400.f)
                    goto rightKick;
                else goto middleKick;
                //only left or right
            }
        }
        action{
            theHeadControlMode = HeadControl::lookLeftAndRight;
            Stand();
        }
    }
    state(leftKick) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            //penaltyKickerAngleLeft();
        }
    }
    state(rightKick) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            penaltyKickerAngleRight();
        }
    }
    state(middleKick) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            //MiddleKick2();
            penaltyKickerAngle();
        }
    }
    state(finish) {
        transition{
            if (state_time > 3000)
                ;
        }
        action{
            theHeadControlMode = HeadControl::lookLeftAndRight;
            Stand();
        }
    }
}
