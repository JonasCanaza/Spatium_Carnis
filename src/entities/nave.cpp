#include "nave.h"

#include <iostream>
#include <cmath>

#include "raylib.h"

#include "utilities/constants.h"

namespace Nave
{
	static void UpdateRotation(Nave& nave);
	static void AccelerateTowardsMouse(Nave& nave, float deltaTime, bool isAccelerating);
	static void CalculateDirectionToMouse(Nave& nave, float& directionX, float& directionY);
	static void ApplyAcceleration(Nave& nave, float directionX, float directionY, float deltaTime);
	static void LimitSpeed(Nave& nave);
	static void Move(Nave& nave, float deltaTime);
	static void WrapAroundScreen(Nave& nave);

	void Init()
	{

	}

	void Update(Nave& nave, float deltaTime, bool isAccelerating)
	{
		UpdateRotation(nave);
		AccelerateTowardsMouse(nave, deltaTime, isAccelerating);
		LimitSpeed(nave);
		Move(nave, deltaTime);
		WrapAroundScreen(nave);
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

	void Reset(Nave& nave)
	{
		nave.x = static_cast<float>(SCREEN_WIDTH / 2);
		nave.y = static_cast<float>(SCREEN_HEIGHT / 2);
		nave.rotation = 0.0f;
		nave.velocityX = 0.0f;
		nave.velocityY = 0.0f;
		nave.lives = 3;
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

	static void AccelerateTowardsMouse(Nave& nave, float deltaTime, bool isAccelerating)
	{
		if (!isAccelerating)
		{
			return;
		}

		float directionX = 0.0f;
		float directionY = 0.0f;

		CalculateDirectionToMouse(nave, directionX, directionY);
		ApplyAcceleration(nave, directionX, directionY, deltaTime);
	}

	static void CalculateDirectionToMouse(Nave& nave, float& directionX, float& directionY)
	{
		float mouseX = static_cast<float>(GetMouseX());
		float mouseY = static_cast<float>(GetMouseY());

		directionX = mouseX - nave.x;
		directionY = mouseY - nave.y;

		float distance = sqrt(directionX * directionX + directionY * directionY);

		if (distance != 0.0f)
		{
			directionX /= distance;
			directionY /= distance;
		}
	}

	static void ApplyAcceleration(Nave& nave, float directionX, float directionY, float deltaTime)
	{
		nave.velocityX += directionX * nave.acceleration * deltaTime;
		nave.velocityY += directionY * nave.acceleration * deltaTime;
	}

	static void LimitSpeed(Nave& nave)
	{
		float speed = sqrt(nave.velocityX * nave.velocityX + nave.velocityY * nave.velocityY);

		if (speed > nave.speedMax)
		{
			nave.velocityX = (nave.velocityX / speed) * nave.speedMax;
			nave.velocityY = (nave.velocityY / speed) * nave.speedMax;
		}
	}

	static void Move(Nave& nave, float deltaTime)
	{
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
}