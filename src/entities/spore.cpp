#include "spore.h"

#include "raylib.h"

#include "game/game_constants.h"
#include "utilities/math_utils.h"

using namespace MathUtils;

namespace Spore
{
	static const float MIN_VELOCITY_X = 25.0f;
	static const float MAX_VELOCITY_X = 100.0f;
	static const float MIN_VELOCITY_Y = 25.0f;
	static const float MAX_VELOCITY_Y = 100.0f;
	static const float MAX_TIME_LIFE = 25.0f;

	static void WrapAroundScreen(Spore& spore);

	void Init()
	{

	}

	void Update(Spore& spore, float deltaTime)
	{
		if (!spore.isActive)
		{
			return;
		}

		spore.x += spore.velocityX * deltaTime;
		spore.y += spore.velocityY * deltaTime;

		WrapAroundScreen(spore);

		spore.timerLife += deltaTime;

		if (spore.timerLife >= MAX_TIME_LIFE)
		{
			spore.isActive = false;
		}
	}

	void Draw(Spore spore)
	{
		if (!spore.isActive)
		{
			return;
		}

		int x = static_cast<int>(spore.x);
		int y = static_cast<int>(spore.y);

		DrawCircle(x, y, spore.radius, GREEN);
	}

	void Close()
	{

	}

	Spore Create()
	{
		Spore newSpore{};

		float x = 0.0f;
		float y = 0.0f;

		newSpore.x = x;
		newSpore.y = y;
		newSpore.radius = 25.0f;
		newSpore.rotation = 0.0f;
		newSpore.velocityX = GetFloatRandomBetween(MIN_VELOCITY_X, MAX_VELOCITY_X);
		newSpore.velocityY = GetFloatRandomBetween(MIN_VELOCITY_Y, MAX_VELOCITY_Y);
		newSpore.timerLife = 0.0f;
		newSpore.isActive = true;

		return newSpore;
	}

	void Reset(Spore& spore)
	{
		float x = 0.0f;
		float y = 0.0f;

		spore.x = x;
		spore.y = y;
		spore.rotation = 0.0f;
		spore.velocityX = GetFloatRandomBetween(MIN_VELOCITY_X, MAX_VELOCITY_X);
		spore.velocityY = GetFloatRandomBetween(MIN_VELOCITY_Y, MAX_VELOCITY_Y);
		spore.timerLife = 0.0f;
		spore.isActive = true;
	}

	static void WrapAroundScreen(Spore& spore)
	{
		if (spore.x + spore.radius < 0)
		{
			spore.x = SCREEN_WIDTH + spore.radius;
		}
		else if (spore.x - spore.radius > SCREEN_WIDTH)
		{
			spore.x = -spore.radius;
		}

		if (spore.y + spore.radius < 0)
		{
			spore.y = SCREEN_HEIGHT + spore.radius;
		}
		else if (spore.y - spore.radius > SCREEN_HEIGHT)
		{
			spore.y = -spore.radius;
		}
	}
}