
float THR_BIG2 = 200.0f;
float THR_LIT2 = 100.0f;
float center_y2 = 0.0f;
float left_y2 = 0.0f;
float right_y2 = 0.0f;
float angle2 = 0.0f;
float big_angle2 = 0.4f;
//float lit_angle2 = 0.1f;
option(penaltyKickerAngleRight) {
    initial_state(start) {
        transition{
            if (state_time > 500) {
                goto walkToball;
            }
        }
        action{
            center_y2 = 0;
            left_y2 = 0;
            right_y2 = 0;
            theHeadControlMode = HeadControl::lookLeftAndRight;
            Stand();
        }
    }

    state(walkToball) {
        transition{
            if (theBallModel.estimate.position.norm() < 200.0f) {
                goto judge;
            }
        }
        action{
            WalkToTarget(Pose2f(0.3f, 0.3f, 0.3f), Pose2f(0.3f, theBallModel.estimate.position.x(), theBallModel.estimate.position.y()));
        }
    }

    state(judge) {
        transition{
            if (state_time > 1000) {
                int count = int(theObstacleModel.obstacles.size());
                if (count == 1) {
                    center_y2 = theObstacleModel.obstacles[0].center.y();
                    left_y2 = theObstacleModel.obstacles[0].left.y();
                    right_y2 = theObstacleModel.obstacles[0].right.y();
                    if (left_y2 < 0 - THR_BIG2) {
                        // PlaySound("heat.wav");
                        // angle2 = lit_angle2;
                        // goto turnRight;
                        goto kickForward;
                    } else if (right_y2 > THR_LIT2) {
                        angle2 = 0.5f;
                        goto turnLeft;
                    } else if (center_y2 < 0 && left_y2 > 0) {
                        angle2 = 0.25f;
                        goto turnLeft;
                    } else if (center_y2 > 0 && right_y2 < 0) {
                        angle2 = 0.4f;
                        goto turnLeft;
                    } else if (left_y2 < 0) {
                        angle2 = big_angle2;
                        goto turnLeft;
                    } else {
                        angle2 = big_angle2;
                        goto turnLeft;
                    }
                    // } else if (count == 2) {
                    //     if ((theObstacleModel.obstacles[0].left.y() < 0 && theObstacleModel.obstacles[1].right.y() > 0)
                    //             || (theObstacleModel.obstacles[1].left.y() < 0 && theObstacleModel.obstacles[0].right.y() > 0)) {
                    //         goto kickForward;//
                    //     } else if ((theObstacleModel.obstacles[0].left.y() < 0 && theObstacleModel.obstacles[1].left.y() < 0)
                    //                || (theObstacleModel.obstacles[0].right.y() > 0 && theObstacleModel.obstacles[1].right.y() > 0)) {
                    //         goto kickForward;
                    //     } else if ((theObstacleModel.obstacles[0].left.y() > 0 && theObstacleModel.obstacles[1].left.y() < 0)) {
                    //         if (theObstacleModel.obstacles[0].right.y() - theObstacleModel.obstacles[1].right.y() > 400) {
                    //             angle2 = lit_angle2;
                    //             goto turnRight;
                    //         } else {
                    //             angle2 = big_angle2;
                    //             goto turnLeft;
                    //         }
                    //     } else if ((theObstacleModel.obstacles[1].left.y() > 0 && theObstacleModel.obstacles[0].left.y() < 0)) {
                    //         if (theObstacleModel.obstacles[1].right.y() - theObstacleModel.obstacles[0].right.y() > 400) {
                    //             angle2 = lit_angle2;
                    //             goto turnRight;
                    //         } else {
                    //             angle2 = big_angle2;
                    //             goto turnLeft;
                    //         }
                    //     }
                }
            }
        }
        action{
            theHeadControlMode = HeadControl::lookLeftAndRight;
            Stand();
        }
    }

    // state(turnRight) {
    //     transition{
    //         if (state_time > 1000 && action_done) {
    //             //goto judge;
    //             goto kickForward;
    //         }
    //     }
    //     action{
    //         PlaySound("allright.wav");
    //         WalkToTarget(Pose2f(0.5f, 0.5f, 0.5f), Pose2f(0 - angle2, 200 * (1.0f - float(cos(angle2))), 200.0f * float(sin(angle2))));
    //     }
    // }

    state(turnLeft) {
        transition{
            if (state_time > 3000 && action_done) {
                //goto judge;
                goto kickForward;
            }
        }
        action{
            PlaySound("lost.wav");
            WalkToTarget(Pose2f(0.5f, 0.5f, 0.5f), Pose2f(angle2, 200 * (1.0f - float(cos(angle2))), -200.0f * float(sin(angle2))));
        }
    }
    state(kickForward) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            KickForward();
        }
    }

    // state(kickForward_left) {
    //     transition{
    //         if (action_done)
    //             goto finish;
    //     }
    //     action{
    //         KickForward_left();
    //     }
    // }

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
