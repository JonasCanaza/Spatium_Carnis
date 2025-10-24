#include "power_up.h"

#include "raylib.h"

#include "game/game_constants.h"

namespace PowerUp
{
	void Init()
	{

	}

	void Update(PowerUP& powerUP, float deltaTime)
	{
		powerUP.x += powerUP.velocityX * deltaTime;
		powerUP.y += powerUP.velocityY * deltaTime;
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
		newPowerUp.velocityX = -50.0f;
		newPowerUp.velocityY = -100.0f;
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
		powerUP.velocityX = -50.0f;
		powerUP.velocityY = -100.0f;
		powerUP.isActive = true;
	}
}