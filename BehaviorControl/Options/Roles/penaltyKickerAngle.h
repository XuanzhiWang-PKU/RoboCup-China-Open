
float THR_BIG = 200.0f;
float THR_LIT = 100.0f;
float center_y = 0.0f;
float left_y = 0.0f;
float right_y = 0.0f;
float angle = 0.0f;
float big_angle = 0.1f;
//float lit_angle = 0.03f;
option(penaltyKickerAngle) {
    initial_state(start) {
        transition{
            if (state_time > 500) {
                goto walkToball;
            }
        }
        action{
            center_y = 0;
            left_y = 0;
            right_y = 0;
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
                    center_y = theObstacleModel.obstacles[0].center.y();
                    left_y = theObstacleModel.obstacles[0].left.y();
                    right_y = theObstacleModel.obstacles[0].right.y();
                    if (left_y < 0 - THR_BIG) {
                        goto kickForward;
                    } else if (right_y > THR_LIT) {
                        goto kickForward;
                    } else if (center_y < 0 && left_y > 0) {
                        angle = big_angle;
                        goto turnLeft;
                    } else if (center_y > 0 && right_y < 0) {
                        angle = big_angle;
                        goto turnRight;
                    } else if (left_y < 0) {
                        //angle = big_angle;
                        goto kickForward;
                    } else {
                        //angle = big_angle;
                        goto kickForward;
                    }
                    // } else if (count == 2) {d
                    //     if ((theObstacleModel.obstacles[0].left.y() < 0 && theObstacleModel.obstacles[1].right.y() > 0)
                    //             || (theObstacleModel.obstacles[1].left.y() < 0 && theObstacleModel.obstacles[0].right.y() > 0)) {
                    //         goto kickForward;//
                    //     } else if ((theObstacleModel.obstacles[0].left.y() < 0 && theObstacleModel.obstacles[1].left.y() < 0)
                    //                || (theObstacleModel.obstacles[0].right.y() > 0 && theObstacleModel.obstacles[1].right.y() > 0)) {
                    //         goto kickForward;
                    //     } else if ((theObstacleModel.obstacles[0].left.y() > 0 && theObstacleModel.obstacles[1].left.y() < 0)) {
                    //         if (theObstacleModel.obstacles[0].right.y() - theObstacleModel.obstacles[1].right.y() > 400) {
                    //             angle = lit_angle;
                    //             goto turnRight;
                    //         } else {
                    //             angle = big_angle;
                    //             goto turnLeft;
                    //         }
                    //     } else if ((theObstacleModel.obstacles[1].left.y() > 0 && theObstacleModel.obstacles[0].left.y() < 0)) {
                    //         if (theObstacleModel.obstacles[1].right.y() - theObstacleModel.obstacles[0].right.y() > 400) {
                    //             angle = lit_angle;
                    //             goto turnRight;
                    //         } else {
                    //             angle = big_angle;
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

        state(turnRight) {
            transition{
                if (state_time > 1100 && action_done) {
                    //goto judge;
                    goto kickForward_left;
                }
            }
            action{
                PlaySound("allright.wav");
                WalkToTarget(Pose2f(0.5f, 0.5f, 0.5f), Pose2f(-0.3f, 5.5, 10));
            }
        }

        state(turnLeft) {
            transition{
                if (state_time > 3000 && action_done) {
                    //goto judge;
                    goto kickForward;
                }
            }
            action{
                PlaySound("lost.wav");
                WalkToTarget(Pose2f(0.5f, 0.5f, 0.5f), Pose2f(angle, 200 * (1.0f - float(cos(angle))), -200.0f * float(sin(angle))));
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

        state(kickForward_left) {
            transition{
                if (action_done)
                    goto finish;
            }
            action{
                KickForward_left();
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
