#include "specimen.h"

#include "raylib.h"

#include "game/game_constants.h"

namespace Specimen
{
	static const float SCREEN_BUFFER = 50.0f;
	static const float HALF_FACTOR = 0.5f;

	const float FRAME_DURATION = 0.1f;
	const int MAX_FRAMES = 5;
	static Texture2D textures[MAX_FRAMES];

	enum FramesID
	{
		FrameOne,
		FrameTwo,
		FrameThree,
		FrameFour,
		FrameFive
	};

	static void WrapAroundScreen(Specimen& specimen);
	static Texture2D GetCurrentTexture(Specimen& specimen);

	void Init()
	{
		textures[FrameOne] = LoadTexture("res/entities/specimen/specimen_01.png");
		textures[FrameTwo] = LoadTexture("res/entities/specimen/specimen_02.png");
		textures[FrameThree] = LoadTexture("res/entities/specimen/specimen_03.png");
		textures[FrameFour] = LoadTexture("res/entities/specimen/specimen_04.png");
		textures[FrameFive] = LoadTexture("res/entities/specimen/specimen_05.png");
	}

	void Update(Specimen& specimen, float deltaTime)
	{
		if (!specimen.isActive)
		{
			return;
		}

		specimen.frameTime += deltaTime;

		if (specimen.frameTime >= FRAME_DURATION)
		{
			specimen.frameTime = 0.0f;
			specimen.currentFrame = (specimen.currentFrame + 1) % MAX_FRAMES;
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

		const Texture2D textureToDraw = GetCurrentTexture(specimen);

		float textureWidth = static_cast<float>(textureToDraw.width);
		float textureHeight = static_cast<float>(textureToDraw.height);

		float halfWidth = textureWidth * HALF_FACTOR;
		float halfHeight = textureHeight * HALF_FACTOR;

		float scaleX = specimen.radius / halfWidth;
		float scaleY = specimen.radius / halfHeight;

		float destX = specimen.x - halfWidth * scaleX;
		float destY = specimen.y - halfHeight * scaleY;

		Rectangle source{ 0.0f, 0.0f, textureWidth, textureHeight };
		Rectangle dest{ destX, destY, textureWidth * scaleX, textureHeight * scaleY };
		Vector2 origin{ 0.0f, 0.0f };

		DrawTexturePro(textureToDraw, source, dest, origin, specimen.rotation, WHITE);
	}

	void Close()
	{
		for (int i = 0; i < MAX_FRAMES; i++)
		{
			UnloadTexture(textures[i]);
		}
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

		newSpecimen.frameTime = 0.0f;
		newSpecimen.currentFrame = 0;

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

	void Reset(Specimen& specimen)
	{
		specimen.isActive = false;
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

	static Texture2D GetCurrentTexture(Specimen& specimen)
	{
		return textures[specimen.currentFrame];
	}
}