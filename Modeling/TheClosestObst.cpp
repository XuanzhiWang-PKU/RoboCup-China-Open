/**
 * @file TheClosestObst.h

 * @author Xuanzhi Wang
 */
#include "Representations/Modeling/TheClosestObst.h"
const Obstacle& TheClosestObst(const ObstacleModel &theObstacleModel) {
    unsigned minDisIdx = 0;
    if (theObstacleModel.obstacles.size() > 0) {
        for (unsigned i = 0; i < theObstacleModel.obstacles.size(); i++) {
            if (theObstacleModel.obstacles[i].center.norm() < theObstacleModel.obstacles[minDisIdx].center.norm()) {
                minDisIdx = i;
            }
        }
        return theObstacleModel.obstacles[minDisIdx];
    }
    else return Obstacle();
}