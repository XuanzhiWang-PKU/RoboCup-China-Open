/**
 * @file SearchForBall.h
 *
 * When the robot couldn't see the ball by itself, we'll communicate with teammates about
 * the position of the ball, if all the teammates couldn't find the ball.the robot will
 * circle at the current posion.If the robot still couldn't find the ball, it would goto the
 * exact position base on the roles.
 *
 * @author Xuanzhi Wang
 */
option(SearchForBall, (const RobotInfo&) theRobotInfo, (float)(1.047f) turnSpeed, (float)(0.3f) commonSpeed) {
    //define the number of the robot
#define STRIKER_NUMBER 1
#define SUPPORTER1_NUMBER 2
#define SUPPORTER2_NUMBER 3
#define DEFENDER_NUMBER 4
#define KEEPER_NUMBER 5
//define the key position
    float STRIKER_X = 750.0f;
    float STRIKER_Y = 0.0f;
    float KEEPER_X = -4000.0f;
    float KEEPER_Y = 0.0f;
    float DEFENDER_X = -2500.0f;
    float DEFENDER_Y = -1500.0f;
    float SUPPORTER1_X = 0.f;
    float SUPPORTER1_Y = -2000.0f;
    float SUPPORTER2_X = -1000.0f;
    float SUPPORTER2_Y = 2000.0f;
    // float OPPONENTGATE_X = 4500.0f;
    // float OPPONENTGATE_Y = 0.0f;
    float x, y; //commom position for different position
    switch (theRobotInfo.number) {
    case 1: x = STRIKER_X, y = STRIKER_Y; break;
    case 2: x = SUPPORTER1_X, y = SUPPORTER1_Y; break;
    case 3: x = SUPPORTER2_X, y = SUPPORTER2_Y; break;
    case 4: x = DEFENDER_X, y = DEFENDER_Y; break;
    case 5: x = KEEPER_X, y = KEEPER_Y; break;
    default: ;
    }
    initial_state(start) {
        transition{

        }
        action{
            switch (theRobotInfo.number) {
            case 1: x = STRIKER_X, y = STRIKER_Y; SearchForBall_1(x, y, turnSpeed, commonSpeed); break;
            case 2: x = SUPPORTER1_X, y = SUPPORTER1_Y; SearchForBall_2(x, y, turnSpeed, commonSpeed); break;
            case 3: x = SUPPORTER2_X, y = SUPPORTER2_Y; SearchForBall_3(x, y, turnSpeed, commonSpeed); break;
            case 4: x = DEFENDER_X, y = DEFENDER_Y; SearchForBall_4(x, y, turnSpeed, commonSpeed); break;
            case 5: x = KEEPER_X, y = KEEPER_Y; SearchForBall_5(x, y, turnSpeed, commonSpeed); break;
            default: ;
            }
        }
    }
}