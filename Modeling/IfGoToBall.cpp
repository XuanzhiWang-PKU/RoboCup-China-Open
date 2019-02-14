/**
 *  当球附近没有对方机器人时候就出击踢球，否则 走到球与球门之间防守。
 *  @author: Xuanzhi Wang
 */
#include "BallModel.h"
#include "Tools/Modeling/Obstacle.h"
#include "Tools/Math/Eigen.h"
#include "RobotPose.h"
#include "Tools/Math/Pose2f.h"
#include "ObstacleModel.h"
#include <cmath>
bool IfGoToBall(const BallModel& theBallModel, const ObstacleModel& theObstacle )
{
	unsigned i=0;
	for( i =0; i < theObstacle.obstacles.size();i++)
	{
		if(theObstacle.obstacles[i].type == Obstacle::opponent || theObstacle.obstacles[i].type == Obstacle::someRobot )
		{
			//if(theObstacle.obstacles[i].center.norm() < 1.6*theBallModel.estimate.position.norm() )
			//		flag=1;
			//if( theObstacle.obstacles[i].center.norm() <  theBallModel.estimate.position.norm() + 500.f  )
			float x=theObstacle.obstacles[i].center.x()-theBallModel.estimate.position.x();
			float y=theObstacle.obstacles[i].center.y()-theBallModel.estimate.position.y();
			if(std::sqrt(x*x+y*y)<theBallModel.estimate.position.norm() + 300.f)
					return false;
		}

	}
	/*
	if(flag==1)
		return false;
	else return true;
	*/
	return true;
}