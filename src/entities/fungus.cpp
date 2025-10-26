#include "fungus.h"

#include "raylib.h"

#include "game/game_constants.h"
#include "utilities/math_utils.h"

namespace Fungus
{
	static const float VELOCITY_MIN = 35.0f;
	static const float VELOCITY_MAX = 70.0f;

	static const float SCREEN_BUFFER = 50.0f;
	static const float RADIUS = 35.0f;

	static void WrapAroundScreen(Fungus& fungus);

	void Init()
	{

	}

	void Update(Fungus& fungus, float deltaTime)
	{
		if (!fungus.isActive)
		{
			return;
		}

		fungus.x += fungus.velocityX * deltaTime;
		fungus.y += fungus.velocityY * deltaTime;

		WrapAroundScreen(fungus);

	}

	void Draw(Fungus fungus)
	{
		if (!fungus.isActive)
		{
			return;
		}

		int x = static_cast<int>(fungus.x);
		int y = static_cast<int>(fungus.y);

		DrawCircle(x, y, fungus.radius, RED);
	}

	void Close()
	{

	}

	Fungus Create(float x, float y, float velocityX, float velocityY)
	{
		Fungus newFungus{};

		newFungus.x = x;
		newFungus.y = y;
		newFungus.radius = RADIUS;
		newFungus.rotation = 0.0f;
		newFungus.velocityX = velocityX;
		newFungus.velocityY = velocityY;
		newFungus.isActive = true;

		return newFungus;
	}

	Fungus SpawnAtSide(SpawnSide side)
	{
		float spawnX = 0.0f;
		float spawnY = 0.0f;
		float velocityX = 0.0f;
		float velocityY = 0.0f;

		switch (side)
		{
		case SpawnSide::Top:

			spawnX = static_cast<float>(rand() % SCREEN_WIDTH);
			spawnY = -SCREEN_BUFFER;
			velocityX = -VELOCITY_MIN + rand() % static_cast<int>(2 * VELOCITY_MIN);
			velocityY = VELOCITY_MIN + rand() % static_cast<int>(VELOCITY_MAX - VELOCITY_MIN);

			break;
		case SpawnSide::Right:

			spawnX = SCREEN_WIDTH + SCREEN_BUFFER;
			spawnY = static_cast<float>(rand() % SCREEN_HEIGHT);
			velocityX = -VELOCITY_MIN - rand() % static_cast<int>(VELOCITY_MAX - VELOCITY_MIN);
			velocityY = -VELOCITY_MIN + rand() % static_cast<int>(2 * VELOCITY_MIN);

			break;
		case SpawnSide::Bottom:

			spawnX = static_cast<float>(rand() % SCREEN_WIDTH);
			spawnY = SCREEN_HEIGHT + SCREEN_BUFFER;
			velocityX = -VELOCITY_MIN + rand() % static_cast<int>(2 * VELOCITY_MIN);
			velocityY = -VELOCITY_MIN - rand() % static_cast<int>(VELOCITY_MAX - VELOCITY_MIN);

			break;
		case SpawnSide::Left:

			spawnX = -SCREEN_BUFFER;
			spawnY = static_cast<float>(rand() % SCREEN_HEIGHT);
			velocityX = VELOCITY_MIN + rand() % static_cast<int>(VELOCITY_MAX - VELOCITY_MIN);
			velocityY = -VELOCITY_MIN + rand() % static_cast<int>(2 * VELOCITY_MIN);

			break;
		default:

			// THERE ARE NO MORE PLACES TO SPAWN

			break;
		}

		return Create(spawnX, spawnY, velocityX, velocityY);
	}

	void Reset(Fungus& fungus)
	{
		fungus.isActive = false;
	}

	static void WrapAroundScreen(Fungus& fungus)
	{
		if (fungus.x + fungus.radius < 0)
		{
			fungus.x = SCREEN_WIDTH + fungus.radius;
		}
		else if (fungus.x - fungus.radius > SCREEN_WIDTH)
		{
			fungus.x = -fungus.radius;
		}

		if (fungus.y + fungus.radius < 0)
		{
			fungus.y = SCREEN_HEIGHT + fungus.radius;
		}
		else if (fungus.y - fungus.radius > SCREEN_HEIGHT)
		{
			fungus.y = -fungus.radius;
		}
	}

}