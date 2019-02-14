/**
 *  Walk to target while avoiding obstacles, use it just as WalkToTarget
 *  (i.e. Walk to a \c target at a \c speed)
 *  @param speed Walking speeds, as a ratio of the maximum speeds [0..1].
 *  @param target Walking target, in mm and radians, relative to the robot.
 *
 *  To avoid swinging among the three walk states, I introduced the idea of double-threshold
 *  To output something only once, I used three static data members to remember the past.
 *  @author: Yuanzhou Xue
 *  @author: Xuanzhi Wang
 */

//#include <stdio.h>

option(WalkToTargetInPathMode, (const Pose2f&) speed, (const Pose2f&) target) {
    // static int i;
    // static int j;
    // static int k;
    initial_state(launch) {
        transition {
            if (state_time > 500) {
                if (theObstacleModel.obstacles.size()) {
                    unsigned minDisIdx = 0;
                    for (unsigned i = 0; i < theObstacleModel.obstacles.size(); i++) {
                        if (theObstacleModel.obstacles[i].center.norm() < theObstacleModel.obstacles[minDisIdx].center.norm()) {
                            minDisIdx = i;
                        }
                    }
                    if (theObstacleModel.obstacles[minDisIdx].center.x() > 0 && theObstacleModel.obstacles[minDisIdx].center.norm() < 1000) {
                        if (theObstacleModel.obstacles[minDisIdx].right.y() >= 250 || theObstacleModel.obstacles[minDisIdx].left.y() <= -250) {
                            goto walkStraight;
                        } else if (theObstacleModel.obstacles[minDisIdx].left.y() <= 0 || (theObstacleModel.obstacles[minDisIdx].left.y() > 0 && theObstacleModel.obstacles[minDisIdx].center.y() <= 0)) {
                            goto walkLeft;
                        } else {
                            goto walkRight;
                        }
                    } else goto walkStraight;
                } else goto walkStraight;
            }
        }
        action {
            // i = 0;
            // j = 0;
            // k = 0;
            LookForward();
        }
    }
    state(walkStraight) {
        transition{
            if (state_time > 500) {
                if (theObstacleModel.obstacles.size()) {
                    unsigned minDisIdx = 0;
                    for (unsigned i = 0; i < theObstacleModel.obstacles.size(); i++) {
                        if (theObstacleModel.obstacles[i].center.norm() < theObstacleModel.obstacles[minDisIdx].center.norm()) {
                            minDisIdx = i;
                        }
                    }
                    if (theObstacleModel.obstacles[minDisIdx].center.x() > -100 && std::abs(theObstacleModel.obstacles[minDisIdx].center.y()) < 500.0f && theObstacleModel.obstacles[minDisIdx].center.norm() < 1000) {
                        if (theObstacleModel.obstacles[minDisIdx].right.y() >= 250 || theObstacleModel.obstacles[minDisIdx].left.y() <= -250) {
                            goto walkStraight;
                        } else if (theObstacleModel.obstacles[minDisIdx].left.y() <= 0 || (theObstacleModel.obstacles[minDisIdx].left.y() > 0 && theObstacleModel.obstacles[minDisIdx].center.y() <= 0)) {
                            goto walkLeft;
                        } else {
                            goto walkRight;
                        }
                    } else goto walkStraight;
                } else goto walkStraight;
            }
        }
        action{
            // j = 0, k = 0;
            // for (; i < 1; i++) {
            //     printf("Size: %zu\n", theObstacleModel.obstacles.size());
            //     printf("Forward\n");
            // }
            WalkToTarget(speed, target);
        }
    }
    state(walkLeft) {
        unsigned minDisIdx = 0;
        transition{
            if (state_time > 500) {
                if (theObstacleModel.obstacles.size()) {
                    for (unsigned i = 0; i < theObstacleModel.obstacles.size(); i++) {
                        if (theObstacleModel.obstacles[i].center.norm() < theObstacleModel.obstacles[minDisIdx].center.norm()) {
                            minDisIdx = i;
                        }
                    }
                    if (theObstacleModel.obstacles[minDisIdx].center.x() > 50 && std::abs(theObstacleModel.obstacles[minDisIdx].center.y()) < 500.0f && theObstacleModel.obstacles[minDisIdx].center.norm() < 1000) {
                        if (theObstacleModel.obstacles[minDisIdx].right.y() >= 300 || theObstacleModel.obstacles[minDisIdx].left.y() <= -300) {
                            goto walkStraight;
                        } else if (theObstacleModel.obstacles[minDisIdx].left.y() <= 50 || (theObstacleModel.obstacles[minDisIdx].left.y() > 50 && theObstacleModel.obstacles[minDisIdx].center.y() <= 50)) {
                            goto walkLeft;
                        } else {
                            goto walkRight;
                        }
                    } else goto walkStraight;
                } else goto walkStraight;
            }
        }
        action{
            // i = 0, k = 0;
            // for (; j < 1; j++) {
            //     printf("Size: %zu\n", theObstacleModel.obstacles.size());
            //     printf("Left\n");
            // }
            // printf("Norm: %f, Left: %f, Right: %f, x: %f\n", theObstacleModel.obstacles[minDisIdx].center.norm(), theObstacleModel.obstacles[minDisIdx].left.y(), theObstacleModel.obstacles[minDisIdx].right.y(), theObstacleModel.obstacles[minDisIdx].center.x());
            WalkToTarget(speed, Pose2f(0.0f, 3000.f, 3000 * 1000 / theObstacleModel.obstacles[minDisIdx].center.x()));
        }
    }
    state(walkRight) {
        unsigned minDisIdx = 0;
        transition{
            if (state_time > 500) {
                if (theObstacleModel.obstacles.size()) {
                    for (unsigned i = 0; i < theObstacleModel.obstacles.size(); i++) {
                        if (theObstacleModel.obstacles[i].center.norm() < theObstacleModel.obstacles[minDisIdx].center.norm()) {
                            minDisIdx = i;
                        }
                    }
                    if (theObstacleModel.obstacles[minDisIdx].center.x() > 50 && std::abs(theObstacleModel.obstacles[minDisIdx].center.y()) < 500.0f && theObstacleModel.obstacles[minDisIdx].center.norm() < 1000) {
                        if (theObstacleModel.obstacles[minDisIdx].right.y() >= 300 || theObstacleModel.obstacles[minDisIdx].left.y() <= -300) {
                            goto walkStraight;
                        } else if (theObstacleModel.obstacles[minDisIdx].left.y() <= -50 || (theObstacleModel.obstacles[minDisIdx].left.y() > -50 && theObstacleModel.obstacles[minDisIdx].center.y() <= -50)) {
                            goto walkLeft;
                        } else {
                            goto walkRight;
                        }
                    } else goto walkStraight;
                } else goto walkStraight;
            }
        }
        action{
            // i = 0, j = 0;
            // for (; k < 1; k++) {
            //     printf("Size: %zu\n", theObstacleModel.obstacles.size());
            //     printf("Right\n");
            // }
            // printf("Norm: %f, Left: %f, Right: %f, x: %f\n", theObstacleModel.obstacles[minDisIdx].center.norm(), theObstacleModel.obstacles[minDisIdx].left.y(), theObstacleModel.obstacles[minDisIdx].right.y(), theObstacleModel.obstacles[minDisIdx].center.x());
            WalkToTarget(speed, Pose2f(0.0f, 3000.f, -3000 * 1000 / theObstacleModel.obstacles[minDisIdx].center.x()));
        }
    }
}