#include "nave.h"

#include <iostream>
#include <cmath>

#include "raylib.h"

#include "utilities/constants.h"

namespace Nave
{
	static void UpdateRotation(Nave& nave);
	static void Move(Nave& nave, float deltaTime);
	static void WrapAroundScreen(Nave& nave);
	static void Shoot(Nave& nave, float deltaTime);

	static bool accelerate = false;
	static bool shoot = false;
	static float reloadTarget = 0.3f;

	void Init()
	{

	}

	void Input()
	{
		accelerate = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
		shoot = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
	}

	void Update(Nave& nave, float deltaTime)
	{
		UpdateRotation(nave);
		Move(nave, deltaTime);
		WrapAroundScreen(nave);
		Shoot(nave, deltaTime);
	}

	void Draw(Nave nave)
	{
		int navePosX = static_cast<int>(nave.x);
		int navePosY = static_cast<int>(nave.y);

		DrawCircle(navePosX, navePosY, nave.radius, RED); // COLLISION

		Rectangle body = { nave.x, nave.y, nave.radius * 2.0f, nave.radius * 2.0f };
		Vector2 originBody = { body.width / 2.0f, body.height / 2.0f };
		DrawRectanglePro(body, originBody, nave.rotation, WHITE);
	}

	void Close()
	{

	}

	Nave Create()
	{
		Nave newNave{};

		newNave.x = static_cast<float>(SCREEN_WIDTH / 2);
		newNave.y = static_cast<float>(SCREEN_HEIGHT / 2);
		newNave.radius = 30.0f;
		newNave.speedMax = 250.0f;
		newNave.acceleration = 250.0f;
		newNave.rotation = 0.0f;
		newNave.velocityX = 0.0f;
		newNave.velocityY = 0.0f;
		newNave.reload = 0.0f;
		newNave.lives = 3;
		newNave.isActive = true;

		return newNave;
	}

	static void UpdateRotation(Nave& nave)
	{
		float mousePosX = static_cast<float>(GetMouseX());
		float mousePosY = static_cast<float>(GetMouseY());

		float deltaX = mousePosX - nave.x;
		float deltaY = mousePosY - nave.y;

		float angleRadians = atan2(deltaY, deltaX);
		float angleDegrees = angleRadians * RAD2DEG;

		nave.rotation = angleDegrees;

		//std::cout << "Mouse X:" << mousePosX << "          Y: " << mousePosY << std::endl;
		//std::cout << "Nave X:" << navePivotX << "          Y: " << navePivotY << std::endl;
		//std::cout << "Rotation: " << nave.rotation << std::endl;
	}

	static void Move(Nave& nave, float deltaTime)
	{
		if (accelerate)
		{
			Vector2 direction = { GetMouseX() - nave.x, GetMouseY() - nave.y };

			float length = sqrt(direction.x * direction.x + direction.y * direction.y);

			if (length != 0.0f)
			{
				direction.x /= length;
				direction.y /= length;
			}

			nave.velocityX += direction.x * nave.acceleration * deltaTime;
			nave.velocityY += direction.y * nave.acceleration * deltaTime;
		}

		float speed = sqrt(nave.velocityX * nave.velocityX + nave.velocityY * nave.velocityY);

		if (speed > nave.speedMax)
		{
			nave.velocityX = (nave.velocityX / speed) * nave.speedMax;
			nave.velocityY = (nave.velocityY / speed) * nave.speedMax;
		}

		nave.x += nave.velocityX * deltaTime;
		nave.y += nave.velocityY * deltaTime;
	}

	static void WrapAroundScreen(Nave& nave)
	{
		if (nave.x + nave.radius < 0)
		{
			nave.x = SCREEN_WIDTH + nave.radius;
		}
		else if (nave.x - nave.radius > SCREEN_WIDTH)
		{
			nave.x = -nave.radius;
		}

		if (nave.y + nave.radius < 0)
		{
			nave.y = SCREEN_HEIGHT + nave.radius;
		}
		else if (nave.y - nave.radius > SCREEN_HEIGHT)
		{
			nave.y = -nave.radius;
		}
	}

	static void Shoot(Nave& nave, float deltaTime)
	{
		if (!shoot)
		{
			nave.reload = 0.0f;
			return;
		}

		nave.reload += deltaTime;

		if (nave.reload >= reloadTarget)
		{
			std::cout << "Shoot!" << std::endl;
			nave.reload = 0.0f;
		}
	}
}