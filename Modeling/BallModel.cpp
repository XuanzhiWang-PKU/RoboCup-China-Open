/**
 * @file BallModel.cpp
 * Implementation of the BallModel's drawing functions
 */

#include "BallModel.h"
#include "Platform/Time.h"
#include "Representations/Configuration/BallSpecification.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Tools/Debugging/DebugDrawings.h"
#include "Tools/Debugging/DebugDrawings3D.h"
#include "Tools/Math/Approx.h"
#include "Tools/Modeling/BallPhysics.h"
#include "Tools/Module/Blackboard.h"

void BallModel::operator>> (BHumanMessage& m) const
{
  m.theBHumanStandardMessage.ballLastPerceptX = static_cast<int16_t>(lastPerception.x());
  m.theBHumanStandardMessage.ballLastPerceptY = static_cast<int16_t>(lastPerception.y());

  m.theBSPLStandardMessage.ball[0] = estimate.position.x();
  m.theBSPLStandardMessage.ball[1] = estimate.position.y();
  m.theBSPLStandardMessage.ballVel[0] = estimate.velocity.x();
  m.theBSPLStandardMessage.ballVel[1] = estimate.velocity.y();

  m.theBHULKsStandardMessage.ballTimeWhenLastSeen = timeWhenLastSeen;
  m.theBHumanStandardMessage.ballTimeWhenDisappearedSeenPercentage = (timeWhenDisappeared & 0x00ffffff) | seenPercentage << 24;

  m.theBHumanStandardMessage.ballCovariance[0] = estimate.covariance(0, 0);
  m.theBHumanStandardMessage.ballCovariance[1] = estimate.covariance(1, 1);
  m.theBHumanStandardMessage.ballCovariance[2] = (estimate.covariance(0, 1) + estimate.covariance(1, 0)) / 2.f;

  if(timeWhenLastSeen && Blackboard::getInstance().exists("FrameInfo"))
  {
    const FrameInfo& theFrameInfo = static_cast<const FrameInfo&>(Blackboard::getInstance()["FrameInfo"]);
    m.theBSPLStandardMessage.ballAge = theFrameInfo.getTimeSince(timeWhenLastSeen) / 1000.f;
  }
  else
    m.theBSPLStandardMessage.ballAge = 1.f;
}

void BallModel::operator<< (const BHumanMessage& m)
{
  estimate.position.x() = m.theBSPLStandardMessage.ball[0];
  estimate.position.y() = m.theBSPLStandardMessage.ball[1];
  estimate.velocity.x() = m.theBSPLStandardMessage.ballVel[0];
  estimate.velocity.y() = m.theBSPLStandardMessage.ballVel[1];

  timeWhenLastSeen = m.toLocalTimestamp(m.theBHULKsStandardMessage.ballTimeWhenLastSeen);

  if(m.theBHULKsStandardMessage.member == B_HUMAN_MEMBER)
  {
    timeWhenDisappeared = m.toLocalTimestamp(m.theBHumanStandardMessage.ballTimeWhenDisappearedSeenPercentage & 0x00ffffff);
    seenPercentage = static_cast<unsigned char>(m.theBHumanStandardMessage.ballTimeWhenDisappearedSeenPercentage >> 24);

    lastPerception.x() = static_cast<float>(m.theBHumanStandardMessage.ballLastPerceptX);
    lastPerception.y() = static_cast<float>(m.theBHumanStandardMessage.ballLastPerceptY);

    estimate.covariance << m.theBHumanStandardMessage.ballCovariance[0], m.theBHumanStandardMessage.ballCovariance[2],
                           m.theBHumanStandardMessage.ballCovariance[2], m.theBHumanStandardMessage.ballCovariance[1];
  }
  else
  {
    timeWhenDisappeared = timeWhenLastSeen;
    seenPercentage = 40;
    lastPerception = estimate.position;
    estimate.covariance << 10000.f, 2000.f, 2000.f, 10000.f;
  }
}

void BallModel::verify() const
{
  ASSERT(std::isfinite(lastPerception.x()));
  ASSERT(std::isfinite(lastPerception.y()));
  ASSERT(seenPercentage <= 100);

  ASSERT(std::isfinite(estimate.position.x()));
  ASSERT(std::isfinite(estimate.position.y()));
  ASSERT(std::isfinite(estimate.velocity.x()));
  ASSERT(std::isfinite(estimate.velocity.y()));

  ASSERT(std::isnormal(estimate.covariance(0, 0)));
  ASSERT(std::isnormal(estimate.covariance(1, 1)));
  ASSERT(std::isfinite(estimate.covariance(0, 1)));
  ASSERT(std::isfinite(estimate.covariance(1, 0)));
  ASSERT(Approx::isEqual(estimate.covariance(0, 1), estimate.covariance(1, 0), 1e-20f));
}

void BallModel::draw() const
{
  // drawing of the ball model in the field view
  DEBUG_DRAWING("representation:BallModel", "drawingOnField")
  {
    const Vector2f& position(estimate.position);
    const Vector2f& velocity(estimate.velocity);
    CIRCLE("representation:BallModel",
           position.x(), position.y(), 45, 0, // pen width
           Drawings::solidPen, ColorRGBA::orange,
           Drawings::solidBrush, ColorRGBA::orange);
    ARROW("representation:BallModel", position.x(), position.y(),
          position.x() + velocity.x(), position.y() + velocity.y(), 5, 1, ColorRGBA::orange);
  }

  DEBUG_DRAWING("representation:BallModel:covariance", "drawingOnField")
  {
    COVARIANCE2D("representation:BallModel:covariance", estimate.covariance, estimate.position);
  }

  // drawing of the end position
  DEBUG_DRAWING("representation:BallModel:endPosition", "drawingOnField")
  {
    if(Blackboard::getInstance().exists("BallSpecification"))
    {
      const BallSpecification& ballSpecification = static_cast<const BallSpecification&>(Blackboard::getInstance()["BallSpecification"]);
      Vector2f position;
      if(estimate.rotation == 0.f)
        position = BallPhysics::getEndPosition(estimate.position, estimate.velocity, ballSpecification.friction);
      else
        position = BallPhysics::getEndPositionRegardingRotation(estimate.position, estimate.velocity, estimate.rotation, ballSpecification.friction);
      ColorRGBA violet = ColorRGBA(168, 25, 99, 220); //So you know what you see in the world state...
      CIRCLE("representation:BallModel:endPosition",
             position.x(),  position.y(), 45, 0, // pen width
             Drawings::solidPen, ColorRGBA::black,
             Drawings::solidBrush, violet);
    }
  }

  DEBUG_DRAWING3D("representation:BallModel", "robot")
  {
    TRANSLATE3D("representation:BallModel", 0, 0, -230);
    if(Time::getTimeSince(timeWhenLastSeen) < 5000 && Time::getTimeSince(timeWhenDisappeared) < 1000)
    {
      SPHERE3D("representation:BallModel", estimate.position.x(), estimate.position.y(), estimate.radius, estimate.radius, ColorRGBA::orange);
      LINE3D("representation:BallModel", 0, 0, 1.f, estimate.position.x(), estimate.position.y(), 1.f, 5.f, ColorRGBA::orange);
    }
  }
}

void GroundTruthBallModel::draw() const
{
  DEBUG_DRAWING("representation:GroundTruthBallModel", "drawingOnField")
  {
    const Vector2f& position(estimate.position);
    const Vector2f& velocity(estimate.velocity);
    ColorRGBA transparentOrange(255, 128, 0, 128);
    CIRCLE("representation:GroundTruthBallModel",
           position.x(), position.y(), 45, 4, // pen width
           Drawings::solidPen, ColorRGBA::gray,
           Drawings::solidBrush, transparentOrange);
    ARROW("representation:GroundTruthBallModel", position.x(), position.y(),
          position.x() + velocity.x(), position.y() + velocity.y(), 5, 1, transparentOrange);
  }
}

void BallModel3D::draw() const
{
  // drawing of the ball model in the field view
  DEBUG_DRAWING("representation:BallModel3D", "drawingOnField")
  {
    const Vector3f& position(estimate.position);
    const Vector3f& velocity(estimate.velocity);
    CIRCLE("representation:BallModel3D",
           position.x(), position.y(), 45, 0, // pen width
           Drawings::solidPen, ColorRGBA::red,
           Drawings::solidBrush, ColorRGBA::red);
    ARROW("representation:BallModel3D", position.x(), position.y(),
          position.x() + velocity.x(), position.y() + velocity.y(), 5, 1, ColorRGBA::red);
  }

  DEBUG_DRAWING3D("representation:BallModel3D", "robot")
  {
    TRANSLATE3D("representation:BallModel3D", 0, 0, -230);
    if(Time::getTimeSince(timeWhenLastSeen) < 5000 && Time::getTimeSince(timeWhenDisappeared) < 1000)
    {
      SPHERE3D("representation:BallModel3D", estimate.position.x(), estimate.position.y(), estimate.position.z(), estimate.radius, ColorRGBA::orange);
    }
  }
}
