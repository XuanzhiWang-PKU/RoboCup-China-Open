/*
 *@author Yuanzhou Xue
 * This option makes the robot look left and right.
 *
 */
#define SPEED 60_deg

option(LookLeftAndRight, (float)(0.525) angle, (float)(0.38f) tilt) {
    initial_state(lookForward) {
        transition{
            if (state_time > 2000) goto lookLeft1;
        }
        action{
            LookForward();
        }
    }
    state(lookLeft1) {
        transition{
            if (state_time > 2000 && action_done) {
                goto lookLeft2;
            }
        }
        action{
            Stand();
            SetHeadPanTilt(angle * 1, tilt, SPEED);
        }
    }
    state(lookLeft2) {
        transition{
            if (state_time > 2000 && action_done) {
                goto lookLeft3;
            }
        }
        action{
            Stand();
            SetHeadPanTilt(angle * 2, tilt, SPEED);
        }
    }
    state(lookLeft3) {
        transition{
            if (state_time > 2000 && action_done) {
                goto lookForward2;
            }
        }
        action{
            Stand();
            SetHeadPanTilt(angle * 1, tilt, SPEED);
        }
    }
    state(lookForward2) {
        transition{
            if (state_time > 2000 && action_done) {
                goto lookRight1;
            }
        }
        action{
            Stand();
            SetHeadPanTilt(angle * 0, tilt, SPEED);
        }
    }
    state(lookRight1) {
        transition{
            if (state_time > 2000 && action_done) {
                goto lookRight2;
            }
        }
        action{
            Stand();
            SetHeadPanTilt(angle * -1, tilt, SPEED);
        }
    }
    state(lookRight2) {
        transition{
            if (state_time > 2000 && action_done) {
                goto lookRight3;
            }
        }
        action{
            Stand();
            SetHeadPanTilt(angle * -2, tilt, SPEED);
        }
    }
    state(lookRight3) {
        transition{
            if (state_time > 2000 && action_done) {
                goto lookForward3;
            }
        }
        action{
            Stand();
            SetHeadPanTilt(angle * -1, tilt, SPEED);
        }
    }
    state(lookForward3) {
        transition{
            if (state_time > 2000 && action_done) {
                goto lookLeft1;
            }
        }
        action{
            Stand();
            SetHeadPanTilt(angle * 0, tilt, SPEED);
        }
    }
}
#undef SPEED