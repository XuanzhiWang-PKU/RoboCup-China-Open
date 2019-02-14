/**
 * @file SearchForBall_5.h
 *
 * This option is for the Keeper.But now all the roles use the same model.
 *
 * @param speed Walking speeds, as a ratio of the maximum speeds [0..1].
 * @param x Walking target, in mm, absolute to the ground.
 * @param x Walking target, in mm, absolute to the ground.
 *
 * @author Xuanzhi Wang
 */
option(SearchForBall_5, (float) x, (float) y, (float)(1.047f) turnSpeed, (float)(0.3f) commonSpeed) {
    float deltadistance = 20.0f;
    initial_state(start) {
        transition{
            if (theTeamBallModel.isValid)
                goto turnToBall;
            else{
                if (theBallModel.lastPerception.y() < 0)
                    goto searchForBallRight;
                else goto searchForBallLeft;

            }

        }
        action{ }
    }
    state(turnToBall) {
        transition {
            if (theTeamBallModel.isValid == false && state_time > 5000)
                goto start;

            if (theTeamBallModel.isValid == true &&
            std::abs(AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()).rotation) < 5_deg)
                goto walkToBall;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed),
            Pose2f(AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()).rotation,
            0.f,
            0.f));
        }
    }
    state(walkToBall) {
        transition {
            if (theTeamBallModel.isValid == false && state_time > 5000)
                goto start;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed),
            AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()));
        }
    }
    state(walkToSomewhere) {
        transition {
            if (theTeamBallModel.isValid)
                goto start;
            if (!theTeamBallModel.isValid)
                goto turnToPosition;
        }
        action {

        }
    }
    state(turnToPosition) {
        transition {
            if (theTeamBallModel.isValid)
                goto start;

            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, x, y);
            if (std::abs(relatePoint.rotation) < 2_deg)
                goto walkToTarget_Ready;
        }
        action {
            LookForward();
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, x, y);
            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed), Pose2f(relatePoint.rotation, 0.0, 0.0));
        }
    }
    state(walkToTarget_Ready) {
        transition {
            if (theTeamBallModel.isValid)
                goto start;
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, x, y);
            float distance = relatePoint.translation.norm();
            if (distance < deltadistance)
                goto start;
            if ((relatePoint.rotation > 20_deg) && (distance > 10 * deltadistance))
                goto turnToPosition;
            if (state_time > 10000)
                goto turnToPosition;
        }
        action {
            //LookLeftAndRight();
            Pose2f relatePoint = AbsolutePointToRobot(theRobotPose, x, y);
            WalkToTarget(Pose2f(turnSpeed, commonSpeed, commonSpeed), Pose2f(0.0f, relatePoint.translation.x(), relatePoint.translation.y()));
        }
    }
    state(searchForBallRight) {
        transition{
            if (theTeamBallModel.isValid == false && state_time > 30000) {
                goto walkToSomewhere;
            }
        }
        action{
            SearchForBallRight();
        }
    }
    state(searchForBallLeft) {
        transition{
            if (theTeamBallModel.isValid == false && state_time > 30000) {
                goto walkToSomewhere;
            }
        }
        action{
            SearchForBallLeft();
        }
    }
}