#include "projectile.h"

#include <cmath>

#include "raylib.h"

#include "nave.h"
#include "game/game_constants.h"
#include <utilities/math_utils.h>

using namespace MathUtils;

namespace Projectile
{
	static Texture texture;

	static const float HALF_FACTOR = 0.5f;
	static const float MAX_ROTATION = 360.0f;

	static const float RADIUS = 10.0f;
	static const float PROJECTILE_SPEED = 1000.0f;

	static const float MIN_ROTATION_RANDOM = 250.0f;
	static const float MAX_ROTATION_RANDOM = 350.0f;

	static void UpdateRotation(Projectile& projectile, float deltaTime);

	void Init()
	{
		texture = LoadTexture("res/textures/entities/projectile/projectile.png");
	}

	void Update(Projectile& projectile, float deltaTime)
	{
		if (!projectile.isActive)
		{
			return;
		}

		projectile.x += projectile.velocityX * deltaTime;
		projectile.y += projectile.velocityY * deltaTime;

		UpdateRotation(projectile, deltaTime);

		if (projectile.x < 0 || projectile.x > SCREEN_WIDTH ||
			projectile.y < 0 || projectile.y > SCREEN_HEIGHT)
		{
			projectile.isActive = false;
		}
	}

	void Draw(Projectile projectile)
	{
		if (!projectile.isActive)
		{
			return;
		}

		float textureWidth = static_cast<float>(texture.width);
		float textureHeight = static_cast<float>(texture.height);

		float halfWidth = textureWidth * HALF_FACTOR;
		float halfHeight = textureHeight * HALF_FACTOR;

		float scaleX = projectile.radius / halfWidth;
		float scaleY = projectile.radius / halfHeight;

		Rectangle source = { 0.0f, 0.0f, textureWidth, textureHeight };
		Rectangle dest = { projectile.x, projectile.y, textureWidth * scaleX, textureHeight * scaleY };
		Vector2 origin = { (textureWidth * scaleX) * HALF_FACTOR, (textureHeight * scaleY) * HALF_FACTOR };

		DrawTexturePro(texture, source, dest, origin, projectile.rotation, WHITE);
		DrawCircleLines(static_cast<int>(projectile.x), static_cast<int>(projectile.y), projectile.radius, RED);
	}

	void Close()
	{
		UnloadTexture(texture);
	}

	Projectile Create(Nave::Nave& nave)
	{
		Projectile newProjectile{};

		newProjectile.x = nave.x;
		newProjectile.y = nave.y;
		newProjectile.radius = RADIUS;
		newProjectile.speedMax = PROJECTILE_SPEED;
		newProjectile.rotation = nave.rotation;
		newProjectile.isActive = true;

		float angleRad = nave.rotation * DEG2RAD;

		newProjectile.velocityX = cos(angleRad) * PROJECTILE_SPEED;
		newProjectile.velocityY = sin(angleRad) * PROJECTILE_SPEED;

		float speed = GetFloatRandomBetween(MIN_ROTATION_RANDOM, MAX_ROTATION_RANDOM);
		int sign = (rand() % 2 == 0) ? 1 : -1;
		newProjectile.rotationSpeed = speed * sign;

		return newProjectile;
	}

	void Reset(Projectile& Projectile)
	{
		Projectile.isActive = false;
	}

	static void UpdateRotation(Projectile& projectile, float deltaTime)
	{
		projectile.rotation += projectile.rotationSpeed * deltaTime;

		if (projectile.rotation > MAX_ROTATION)
		{
			projectile.rotation -= MAX_ROTATION;
		}
		else if (projectile.rotation < 0.0f)
		{
			projectile.rotation += MAX_ROTATION;
		}
	}
}