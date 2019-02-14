/** Sets all members of the MotionRequest representation for executing a targetMode-WalkRequest
 *  (i.e. Walk to a \c target at a \c speed)
 *  @param speed Walking speeds, as a ratio of the maximum speeds [0..1].
 *  @param target Walking target, in mm and radians, relative to the robot.
 *  @param excludePenaltyArea Avoid entering own penality area or not.
 * @author Xuanzhi Wang
 */
MotionRequest plannedRequest;
option(PathToTarget, (const Pose2f&) speed, (const Pose2f&) target, (const bool)(true) excludePenaltyArea) {
    /** Set the motion request. */
    initial_state(launch) {
        transition {
            if (theMotionInfo.motion == MotionRequest::walk)
                goto execute;
        }
        action {
            theMotionRequest = thePathPlanner.plan(speed, target, excludePenaltyArea);
        }
    }
    /** The motion process has started executing the request. */
    target_state(execute) {
        transition {
            if (theMotionInfo.motion != MotionRequest::walk)
                goto launch;
        }
        action {
            theMotionRequest = thePathPlanner.plan(speed, target, excludePenaltyArea);
        }
    }
}
