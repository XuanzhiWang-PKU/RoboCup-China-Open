/**
 * @file SearchForBallRight.h
 *
 * Search for ball from right,each 30_deg followed by 2s pause.
 *
 *@param perAngle the angle the robot will turn beteewn two pause.
 *
 * @author Xuanzhi Wang
 */
#include <stdio.h>

#define PI 3.1415926
option(SearchForBallRight, (float)(-0.523599) perAngle) {
    initial_state(start) {
        transition{
            goto stand;
        }
        action{
            LookForward();
            Stand();
        }
    }
    state(stand) {
        transition{

            if (state_time > 2000)
                goto turnLeft;
        }
        action{
            LookForward();
            Stand();
        }
    }
    state(turnLeft) {
        transition{
            if (state_time > 2000)
                goto stand;
        }
        action{
            LookForward();
            WalkToTarget(Pose2f(1.f, 1.f, 1.f), Pose2f(perAngle, 0.f, 0.f));
        }
    }
}