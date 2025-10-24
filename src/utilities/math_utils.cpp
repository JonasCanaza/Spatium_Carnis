#include "math_utils.h"

#include <stdlib.h>

namespace MathUtils
{
	float GetFloatRandomBetween(float min, float max)
	{
        float randomNormalized = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float range = max - min;
        float randomValue = min + randomNormalized * range;

        return randomValue;
	}

    int GetIntegerRandomBetween(int min, int max)
    {
        int range = max - min + 1;
        int randomOffset = rand() % range;
        int randomValue = min + randomOffset;

        return randomValue;
    }
}