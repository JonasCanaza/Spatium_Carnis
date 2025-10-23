#include "collisions.h"

namespace Collisions
{
    bool CheckCircleCollision(float x1, float y1, float radius1, float x2, float y2, float radius2)
    {
        float deltaX = x2 - x1;
        float deltaY = y2 - y1;

        float distanceSquared = (deltaX * deltaX) + (deltaY * deltaY);
        float combinedRadius = radius1 + radius2;
        float combinedRadiusSquared = combinedRadius * combinedRadius;

        return distanceSquared <= combinedRadiusSquared;
    }
}