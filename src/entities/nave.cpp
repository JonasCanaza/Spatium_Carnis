#include "nave.h"

#include <iostream>
#include <cmath>

#include "raylib.h"

#include "utilities/constants.h"

namespace Nave
{
	static void UpdateRotation(Nave& nave);
	static void Move();

	void Init()
	{

	}

	void Input()
	{

	}

	void Update(Nave& nave)
	{
		UpdateRotation(nave);
		Move();
	}

	void Draw(Nave nave)
	{
		int navePosX = static_cast<int>(nave.x);
		int navePosY = static_cast<int>(nave.y);

		DrawCircle(navePosX, navePosY, nave.radius, RED); // COLLISION

		Rectangle body = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, nave.radius * 2.0f, nave.radius * 2.0f };
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
		std::cout << "Rotation: " << nave.rotation << std::endl;
	}

	static void Move()
	{

	}
}