#include "power_up.h"

#include "raylib.h"

#include "game/game_constants.h"
#include "utilities/math_utils.h"

using namespace MathUtils;

namespace PowerUp
{
	static const float MIN_VELOCITY_X = 25.0f;
	static const float MAX_VELOCITY_X = 100.0f;
	static const float MIN_VELOCITY_Y = 25.0f;
	static const float MAX_VELOCITY_Y = 100.0f;
	static const float MAX_TIME_LIFE = 25.0f;

	static void WrapAroundScreen(PowerUP& powerUP);

	void Init()
	{

	}

	void Update(PowerUP& powerUP, float deltaTime)
	{
		if (!powerUP.isActive)
		{
			return;
		}

		powerUP.x += powerUP.velocityX * deltaTime;
		powerUP.y += powerUP.velocityY * deltaTime;

		WrapAroundScreen(powerUP);

		powerUP.timerLife += deltaTime;

		if (powerUP.timerLife >= MAX_TIME_LIFE)
		{
			powerUP.isActive = false;
		}
	}

	void Draw(PowerUP powerUP)
	{
		if (!powerUP.isActive)
		{
			return;
		}

		int x = static_cast<int>(powerUP.x);
		int y = static_cast<int>(powerUP.y);

		DrawCircle(x, y, powerUP.radius, GREEN);
	}

	void Close()
	{

	}

	PowerUP Create()
	{
		PowerUP newPowerUp{};

		float x = 0.0f;
		float y = 0.0f;

		newPowerUp.x = x;
		newPowerUp.y = y;
		newPowerUp.radius = 25.0f;
		newPowerUp.rotation = 0.0f;
		newPowerUp.velocityX = GetFloatRandomBetween(MIN_VELOCITY_X, MAX_VELOCITY_X);
		newPowerUp.velocityY = GetFloatRandomBetween(MIN_VELOCITY_Y, MAX_VELOCITY_Y);
		newPowerUp.timerLife = 0.0f;
		newPowerUp.isActive = true;

		return newPowerUp;
	}

	void Reset(PowerUP& powerUP)
	{
		float x = 0.0f;
		float y = 0.0f;

		powerUP.x = x;
		powerUP.y = y;
		powerUP.rotation = 0.0f;
		powerUP.velocityX = GetFloatRandomBetween(MIN_VELOCITY_X, MAX_VELOCITY_X);
		powerUP.velocityY = GetFloatRandomBetween(MIN_VELOCITY_Y, MAX_VELOCITY_Y);
		powerUP.timerLife = 0.0f;
		powerUP.isActive = true;
	}

	static void WrapAroundScreen(PowerUP& powerUP)
	{
		if (powerUP.x + powerUP.radius < 0)
		{
			powerUP.x = SCREEN_WIDTH + powerUP.radius;
		}
		else if (powerUP.x - powerUP.radius > SCREEN_WIDTH)
		{
			powerUP.x = -powerUP.radius;
		}

		if (powerUP.y + powerUP.radius < 0)
		{
			powerUP.y = SCREEN_HEIGHT + powerUP.radius;
		}
		else if (powerUP.y - powerUP.radius > SCREEN_HEIGHT)
		{
			powerUP.y = -powerUP.radius;
		}
	}
}