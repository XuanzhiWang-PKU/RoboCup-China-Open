#include "BallModel.h"
#include "Tools/Modeling/Obstacle.h"
#include "Tools/Math/Eigen.h"
#include "RobotPose.h"
#include "Tools/Math/Pose2f.h"
#include "ObstacleModel.h"
#include "IfOppoInFront.h"

bool IfFindGoal(const ObstacleModel& theObstacle )
{
	unsigned i=0;
	for( i =0; i < theObstacle.obstacles.size();i++)
	{
        if(theObstacle.obstacles[i].type == Obstacle::goalpost)
    		return true;
	}
	return false;
    
}
