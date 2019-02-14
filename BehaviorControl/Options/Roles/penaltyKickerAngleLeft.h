
// float THR_BIG1 = 200.0f;
// float THR_LIT1 = 100.0f;
// float center_y1 = 0.0f;
// float left_y1 = 0.0f;
// float right_y1 = 0.0f;
// float angle1 = 0.0f;
// float big_angle1 = 0.25f;
// //float lit_angle1 = 0.1f;
// option(penaltyKickerAngleLeft) {
//     initial_state(start) {
//         transition{
//             if (state_time > 500) {
//                 goto walkToball;
//             }
//         }
//         action{
//             center_y1 = 0;
//             left_y1 = 0;
//             right_y1 = 0;
//             theHeadControlMode = HeadControl::lookLeftAndRight;
//             Stand();
//         }
//     }

//     state(walkToball) {
//         transition{
//             if (theBallModel.estimate.position.norm() < 200.0f) {
//                 goto judge;
//             }
//         }
//         action{
//             WalkToTarget(Pose2f(0.3f, 0.3f, 0.3f), Pose2f(0.3f, theBallModel.estimate.position.x(), theBallModel.estimate.position.y()));
//         }
//     }

//     state(judge) {
//         transition{
//             if (state_time > 1000) {
//                 int count = int(theObstacleModel.obstacles.size());
//                 if (count == 1) {
//                     center_y1 = theObstacleModel.obstacles[0].center.y();
//                     left_y1 = theObstacleModel.obstacles[0].left.y();
//                     right_y1 = theObstacleModel.obstacles[0].right.y();
//                     if (left_y1 < 0 - THR_BIG1) {
//                         angle1 = 0.3f;
//                         goto turnRight;
//                         //goto kickForward;
//                     } else if (right_y1 > THR_LIT1) {
//                         //ngle = 0.5f;
//                         //goto turnLeft;
//                         goto kickForward;
//                     } else if (center_y1 < 0 && left_y1 > 0) {
//                         //angle1 = big_angle1;
//                         //goto turnLeft;
//                         goto kickForward;
//                     } else if (center_y1 > 0 && right_y1 < 0) {
//                         angle1 = big_angle1;
//                         goto turnRight;
//                     } else if (left_y1 < 0) {
//                         goto kickForward;
//                     } else {
//                         angle1 = 0.3f;
//                         goto turnRight;
//                     }
//                 }
//             }
//         }
//         action{
//             theHeadControlMode = HeadControl::lookLeftAndRight;
//             Stand();
//         }
//     }

//     state(turnRight) {
//         transition{
//             if (state_time > 1000 && action_done) {
//                 //goto judge;
//                 goto kickForward;
//             }
//         }
//         action{
//             PlaySound("allright.wav");
//             WalkToTarget(Pose2f(0.5f, 0.5f, 0.5f), Pose2f(0 - angle1, 200 * (1.0f - float(cos(angle1))), 200.0f * float(sin(angle1))));
//         }
//     }

//     // state(turnLeft) {
//     //     transition{
//     //         if (state_time > 3000 && action_done) {
//     //             //goto judge;
//     //             goto kickForward;
//     //         }
//     //     }
//     //     action{
//     //         PlaySound("lost.wav");
//     //         WalkToTarget(Pose2f(0.5f, 0.5f, 0.5f), Pose2f(angle1, 200 * (1.0f - float(cos(angle1))), -200.0f * float(sin(angle1))));
//     //     }
//     // }
//     state(kickForward) {
//         transition{
//             if (action_done)
//                 goto finish;
//         }
//         action{
//             KickForward();
//         }
//     }

//     // state(kickForward_left) {
//     //     transition{
//     //         if (action_done)
//     //             goto finish;
//     //     }
//     //     action{
//     //         KickForward_left();
//     //     }
//     // }

//     state(finish) {
//         transition{
//             if (state_time > 3000)
//                 ;
//         }
//         action{
//             theHeadControlMode = HeadControl::lookLeftAndRight;
//             Stand();
//         }
//     }
// }
