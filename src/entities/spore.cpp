#include "spore.h"

#include "raylib.h"

#include "game/game_constants.h"
#include "utilities/math_utils.h"

using namespace MathUtils;

namespace Spore
{
	static Texture texture;

	static const float HALF_FACTOR = 0.5f;
	static const float MAX_ROTATION = 360.0f;

	static const float MIN_VELOCITY_X = 25.0f;
	static const float MAX_VELOCITY_X = 100.0f;
	static const float MIN_VELOCITY_Y = 25.0f;
	static const float MAX_VELOCITY_Y = 100.0f;
	static const float MAX_TIME_LIFE = 30.0f;

	static const float SCREEN_BUFFER = 50.0f;
	static const float RADIUS = 25.0f;
	static const float MIN_ROTATION_RANDOM = 25.0f;
	static const float MAX_ROTATION_RANDOM = 50.0f;

	static void UpdateRotation(Spore& spore, float deltaTime);
	static void WrapAroundScreen(Spore& spore);

	void Init()
	{
		texture = LoadTexture("res/textures/entities/spore/spore.png");
	}

	void Update(Spore& spore, float deltaTime)
	{
		if (!spore.isActive)
		{
			return;
		}

		spore.x += spore.velocityX * deltaTime;
		spore.y += spore.velocityY * deltaTime;

		UpdateRotation(spore, deltaTime);
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

		float textureWidth = static_cast<float>(texture.width);
		float textureHeight = static_cast<float>(texture.height);

		float halfWidth = textureWidth * HALF_FACTOR;
		float halfHeight = textureHeight * HALF_FACTOR;

		float scaleX = spore.radius / halfWidth;
		float scaleY = spore.radius / halfHeight;

		Rectangle source = { 0.0f, 0.0f, textureWidth, textureHeight };
		Rectangle dest = { spore.x, spore.y, textureWidth * scaleX, textureHeight * scaleY };
		Vector2 origin = { (textureWidth * scaleX) * HALF_FACTOR, (textureHeight * scaleY) * HALF_FACTOR };

		DrawTexturePro(texture, source, dest, origin, spore.rotation, WHITE);
	}

	void Close()
	{
		UnloadTexture(texture);
	}

	Spore Create(float x, float y, float velocityX, float velocityY)
	{
		Spore newSpore{};

		newSpore.x = x;
		newSpore.y = y;
		newSpore.radius = RADIUS;
		newSpore.rotation = 0.0f;
		newSpore.velocityX = velocityX;
		newSpore.velocityY = velocityY;
		newSpore.timerLife = 0.0f;
		newSpore.isActive = true;

		float speed = GetFloatRandomBetween(MIN_ROTATION_RANDOM, MAX_ROTATION_RANDOM);
		int sign = (rand() % 2 == 0) ? 1 : -1;
		newSpore.rotationSpeed = speed * sign;

		return newSpore;
	}

	Spore SpawnAtSide(SpawnSide side)
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

	void Reset(Spore& spore)
	{
		spore.isActive = false;
	}

	static void UpdateRotation(Spore& spore, float deltaTime)
	{
		spore.rotation += spore.rotationSpeed * deltaTime;

		if (spore.rotation > MAX_ROTATION)
		{
			spore.rotation -= MAX_ROTATION;
		}
		else if (spore.rotation < 0.0f)
		{
			spore.rotation += MAX_ROTATION;
		}
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