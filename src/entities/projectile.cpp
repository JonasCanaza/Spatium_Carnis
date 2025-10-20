#include "projectile.h"

#include <iostream>
#include <cmath>

#include "raylib.h"

#include "nave.h"
#include "utilities/constants.h"

namespace Projectile
{
	static const float PROJECTILE_SPEED = 1000.0f;

	void Init()
	{

	}

	void Update(Projectile& projectile, float deltaTime)
	{
		if (!projectile.isActive)
		{
			return;
		}

		projectile.x += projectile.velocityX * deltaTime;
		projectile.y += projectile.velocityY * deltaTime;

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

		int x = static_cast<int>(projectile.x);
		int y = static_cast<int>(projectile.y);

		DrawCircle(x, y, projectile.radius, YELLOW);
	}

	void Close()
	{

	}

	Projectile Create(Nave::Nave& nave)
	{
		Projectile newProjectile{};

		newProjectile.x = nave.x;
		newProjectile.y = nave.y;
		newProjectile.radius = 10.0f;
		newProjectile.speedMax = PROJECTILE_SPEED;
		newProjectile.rotation = nave.rotation;
		newProjectile.isActive = true;

		float angleRad = nave.rotation * DEG2RAD;

		newProjectile.velocityX = cos(angleRad) * PROJECTILE_SPEED;
		newProjectile.velocityY = sin(angleRad) * PROJECTILE_SPEED;

		return newProjectile;
	}
}