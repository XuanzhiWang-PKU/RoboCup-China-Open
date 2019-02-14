option(FocusBall, (float) (0.38f) tilt) {
    /** Simply sets the necessary angles */
    initial_state(lookForward) {
        action {
            float pan = theBallModel.estimate.position.angle();
            //float pan = AbsolutePointToRobot(theRobotPose, theTeamBallModel.position.x(), theTeamBallModel.position.y()).rotation;
            if (std::abs(pan) > 2) pan = 2;
            SetHeadPanTilt( pan , tilt, 300_deg);
        }
    }
}
