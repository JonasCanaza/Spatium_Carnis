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
	static const float MAX_TIME_LIFE = 15.0f;

	void Init()
	{

	}

	void Update(PowerUP& powerUP, float deltaTime)
	{
		powerUP.x += powerUP.velocityX * deltaTime;
		powerUP.y += powerUP.velocityY * deltaTime;

		powerUP.timerLife += deltaTime;

		if (powerUP.timerLife >= MAX_TIME_LIFE)
		{
			powerUP.isActive = false;
		}
	}

	void Draw(PowerUP powerUP)
	{
		int x = static_cast<int>(powerUP.x);
		int y = static_cast<int>(powerUP.y);

		DrawCircle(x, y, powerUP.radius, WHITE);
	}

	void Close()
	{

	}

	PowerUP Create()
	{
		PowerUP newPowerUp{};

		float x = static_cast<float>(SCREEN_WIDTH / 2);
		float y = static_cast<float>(SCREEN_HEIGHT / 2);

		newPowerUp.x = x;
		newPowerUp.y = y;
		newPowerUp.radius = 30.0f;
		newPowerUp.rotation = 0.0f;
		newPowerUp.velocityX = GetFloatRandomBetween(MIN_VELOCITY_X, MAX_VELOCITY_X);
		newPowerUp.velocityY = GetFloatRandomBetween(MIN_VELOCITY_Y, MAX_VELOCITY_Y);
		newPowerUp.timerLife = 0.0f;
		newPowerUp.isActive = true;

		return newPowerUp;
	}

	void Reset(PowerUP& powerUP)
	{
		float x = static_cast<float>(SCREEN_WIDTH / 2);
		float y = static_cast<float>(SCREEN_HEIGHT / 2);

		powerUP.x = x;
		powerUP.y = y;
		powerUP.rotation = 0.0f;
		powerUP.velocityX = GetFloatRandomBetween(MIN_VELOCITY_X, MAX_VELOCITY_X);
		powerUP.velocityY = GetFloatRandomBetween(MIN_VELOCITY_Y, MAX_VELOCITY_Y);
		powerUP.timerLife = 0.0f;
		powerUP.isActive = true;
	}
}