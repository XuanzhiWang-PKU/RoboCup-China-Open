/**
 * @file GlobalFieldCoverage.cpp
 * @author Andreas Stolpmann
 */

#include "GlobalFieldCoverage.h"
#include "Tools/Debugging/DebugDrawings.h"
#include <limits>

GlobalFieldCoverage::Cell::Cell(const int coverage, unsigned timestamp,
                                const float positionOnFieldX, const float positionOnFieldY,
                                const float cellLengthX, const float cellLengthY)
  : coverage(coverage), timestamp(timestamp), positionOnField(positionOnFieldX, positionOnFieldY)
{
  const float xMin = positionOnFieldX - cellLengthX / 2.f;
  const float xMax = positionOnFieldX + cellLengthX / 2.f;
  const float yMin = positionOnFieldY - cellLengthY / 2.f;
  const float yMax = positionOnFieldY + cellLengthY / 2.f;

  const_cast<Vector2f&>(polygon[0]) = Vector2f(xMin + 25.0f, yMin + 25.0f);
  const_cast<Vector2f&>(polygon[1]) = Vector2f(xMin + 25.0f, yMax - 25.0f);
  const_cast<Vector2f&>(polygon[2]) = Vector2f(xMax - 25.0f, yMax - 25.0f);
  const_cast<Vector2f&>(polygon[3]) = Vector2f(xMax - 25.0f, yMin + 25.0f);
}

void GlobalFieldCoverage::draw() const
{
  DECLARE_DEBUG_DRAWING("representation:GlobalFieldCoverage:coverageNumbers", "drawingOnField");
  DEBUG_DRAWING("representation:GlobalFieldCoverage:coverage", "drawingOnField")
  {
    int min = std::numeric_limits<int>::max();
    int max = std::numeric_limits<int>::min();
    for(size_t i = 0; i < grid.size(); ++i)
    {
      if(min > grid[i].coverage)
        min = grid[i].coverage;
      if(max < grid[i].coverage)
        max = grid[i].coverage;
    }

    for(size_t i = 0; i < grid.size(); ++i)
    {
      const Cell& c = grid[i];

      const unsigned char alpha = static_cast<unsigned char>((1.f - (static_cast<float>(c.coverage - min) / static_cast<float>(max - min))) * 255.f);
      QUADRANGLE("representation:GlobalFieldCoverage:coverage",
                 c.polygon[0].x(), c.polygon[0].y(),
                 c.polygon[1].x(), c.polygon[1].y(),
                 c.polygon[2].x(), c.polygon[2].y(),
                 c.polygon[3].x(), c.polygon[3].y(),
                 20, Drawings::solidPen, ColorRGBA(255, 0, 0, alpha));

      DRAWTEXT("representation:GlobalFieldCoverage:coverageNumbers", c.polygon[0].x(), c.polygon[0].y(), 100, ColorRGBA(255, 255, 255, 255), c.coverage / 1000);
    }
  }
}