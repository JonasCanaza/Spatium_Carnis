#include "specimen.h"

#include "raylib.h"

#include "utilities/constants.h"

namespace Specimen
{
	static const float SCREEN_BUFFER = 50.0f;
	static const float VELOCITY_MIN = 50.0f;
	static const float VELOCITY_MAX = 150.0f;

	static void WrapAroundScreen(Specimen& specimen);

	void Init()
	{

	}

	void Update(Specimen& specimen, float deltaTime)
	{
		if (!specimen.isActive)
		{
			return;
		}

		specimen.x += specimen.velocityX * deltaTime;
		specimen.y += specimen.velocityY * deltaTime;

		WrapAroundScreen(specimen);
	}

	void Draw(Specimen specimen)
	{
		if (!specimen.isActive)
		{
			return;
		}

		int x = static_cast<int>(specimen.x);
		int y = static_cast<int>(specimen.y);

		DrawCircle(x, y, specimen.radius, GRAY);
	}

	void Close()
	{

	}

	Specimen Create(float x, float y, float velocityX, float velocityY, Type type)
	{
		Specimen newSpecimen{};

		newSpecimen.x = x;
		newSpecimen.y = y;
		newSpecimen.velocityX = velocityX;
		newSpecimen.velocityY = velocityY;
		newSpecimen.rotation = 0.0f;
		newSpecimen.isActive = true;
		newSpecimen.type = type;

		switch (type)
		{
		case Type::Big:

			newSpecimen.radius = 50.0f;

			break;
		case Type::Medium:

			newSpecimen.radius = 35.0f;

			break;
		case Type::Small:

			newSpecimen.radius = 20.0f;

			break;
		default:

			// THERE ARE NO MORE TYPES OF SPECIMENS

			break;
		}

		return newSpecimen;
	}

	Specimen SpawnAtSide(SpawnSide side, Type type)
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

		return Create(spawnX, spawnY, velocityX, velocityY, type);
	}

	static void WrapAroundScreen(Specimen& specimen)
	{
		if (specimen.x + specimen.radius < 0)
		{
			specimen.x = SCREEN_WIDTH + specimen.radius;
		}
		else if (specimen.x - specimen.radius > SCREEN_WIDTH)
		{
			specimen.x = -specimen.radius;
		}

		if (specimen.y + specimen.radius < 0)
		{
			specimen.y = SCREEN_HEIGHT + specimen.radius;
		}
		else if (specimen.y - specimen.radius > SCREEN_HEIGHT)
		{
			specimen.y = -specimen.radius;
		}
	}
}