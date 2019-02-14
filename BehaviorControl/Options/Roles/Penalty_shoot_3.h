#include <cmath>
#include <cstdio>
//the stragety of penalty shoot according to the position of the obstacles
//float THR_BIG1 = 200.0f;
//float THR_LIT1 = 100.0f;
float center_y1 = 0.0f;
float left_y1 = 0.0f;
float right_y1 = 0.0f;
float angle1 = 0.0f;
//float big_angle1 = 0.25f;
//float distance=0.f;
//float lit_angle1 = 0.1f;
option(Penalty_shoot_3) {
    initial_state(start) {
        transition{
            if (state_time > 500) {
                goto walkToball;
            }
        }
        action{
            center_y1 = 0;
            left_y1 = 0;
            right_y1 = 0;
            //distance=0;
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
                if(count==0)
                {
                    goto prepare;
                }
                for(int i=0;i<count;i++)
                {
                    ShowBallModel(theBallModel);
                    ShowObstacleModel(theObstacleModel.obstacles[i]);
                    if(std::fabs(theObstacleModel.obstacles[i].center.y()-theBallModel.estimate.position.y())<300.f)
                    {
                        //OUTPUT_TEXT("%f,%f\n",theObstacleModel.obstacles[i].center.y(),theBallModel.estimate.position.y());
                        center_y1 = theObstacleModel.obstacles[i].center.y();
                        left_y1 = theObstacleModel.obstacles[i].left.y();
                        right_y1 = theObstacleModel.obstacles[i].right.y();
                        
                        goto prepare;
                        //distance=theObstacleModel.obstacles[i].
                    }
                }
                
            }
        }
        action{
            theHeadControlMode = HeadControl::lookLeftAndRight;
            Stand();

        }
    }

    state(prepare)
    {
        transition{
            angle1=std::atan(std::fabs(right_y1-center_y1)/400.f);
            if(center_y1-theBallModel.estimate.position.y()>=0)
            {
                angle=-1*angle1; 
                goto shoot_striker_alignBehindBall_right;
                
            }
            else 
            {
                goto shoot_striker_alignBehindBall_left;
                
            }
        }
        action{
            Stand();
            theHeadControlMode=HeadControl::lookForward;
        }
    }
    state(shoot_striker_alignBehindBall_right) {
        transition {
            if ((theLibCodeRelease.between(theBallModel.estimate.position.y(), 30.f, 50.f)
            && theLibCodeRelease.between(theBallModel.estimate.position.x(), 190.f, 220.f)
            //&&std::abs(theLibCodeRelease.angleToGoal) < std::fabs(angle1) &&std::abs(theLibCodeRelease.angleToGoal) > 12_deg)
            //&& std::fabs(theRobotPose.rotation-angle1)<3_deg
            && std::fabs(theRobotPose.rotation)>10_deg)
            ||state_time>6000.f
            )
                goto shoot_striker_kick_right;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(0.2f, 0.2f, 0.2f),
            Pose2f( angle1,
            theBallModel.estimate.position.x() - 200.f,//150
            theBallModel.estimate.position.y() - 30.f));
        }
    }
    state(shoot_striker_alignBehindBall_left) {
        transition {
            if ((theLibCodeRelease.between(theBallModel.estimate.position.y(), -50.0f, -30.0f)
            && theLibCodeRelease.between(theBallModel.estimate.position.x(), 190.f, 220.f)
            //&&std::abs(theLibCodeRelease.angleToGoal) < std::fabs(angle1) &&std::abs(theLibCodeRelease.angleToGoal) >12_deg)
            //&& std::fabs(theRobotPose.rotation-angle1)<3_deg
            && std::fabs(theRobotPose.rotation)>10_deg)
            || state_time>6000.f
            )
                goto shoot_striker_kick_left;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            ShowBallPose(theBallModel);
            WalkToTarget(Pose2f(0.2f, 0.2f, 0.2f),
            Pose2f( -1*angle1,
            theBallModel.estimate.position.x() - 200.f,//150
            theBallModel.estimate.position.y() + 30.f));
        }
    }

    state(shoot_striker_kick_right) {
        transition {
            if (state_time > 10000 || (state_time > 2000 && action_done))
                goto finish;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            Kick(KickRequest::BQ_Kick, true);
        }
    }

    state(shoot_striker_kick_left) {
        transition {
            if (state_time > 10000 || (state_time > 2000 && action_done))
                goto finish;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            Kick(KickRequest::BQ_Kick, false);
        }
    }
    
    // state(kickForward) {
    //     transition{
    //         if (action_done)
    //             goto finish;
    //     }
    //     action{
    //         KickForward();
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
