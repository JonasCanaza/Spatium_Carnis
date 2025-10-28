#include "nave.h"

#include <cmath>

#include "raylib.h"

#include "game/game_constants.h"
#include "utilities/math_utils.h"

using namespace MathUtils;

namespace Nave
{
	static Texture texture;

	static const int DEFAULT_LIFE = 3;
	static const int INCREASE_LIFE = 1;
	static const int REDUCE_LIFE = 1;
	static const int MAX_LIFE = 6;

	static const float DEFAULT_FIRE_RATE = 0.25f;
	static const float REDUCE_FIRE_RATE = 0.05f;
	static const float MAX_FIRE_RATE = 0.10f;

	static const float DEFAULT_SPEED = 250.0f;
	static const float INCREASE_SPEED = 50.0f;
	static const float MAX_SPEED = 500.0f;

	static const float MAX_IMMUNITY_TIME = 15.0f;

	static const int MIN_SCORE = 25;
	static const int MAX_SCORE = 50;

	static void UpdateRotation(Nave& nave);
	static void AccelerateTowardsMouse(Nave& nave, float deltaTime, bool isAccelerating);
	static void CalculateDirectionToMouse(Nave& nave, float& directionX, float& directionY);
	static void ApplyAcceleration(Nave& nave, float directionX, float directionY, float deltaTime);
	static void LimitSpeed(Nave& nave);
	static void Move(Nave& nave, float deltaTime);
	static void WrapAroundScreen(Nave& nave);
	static bool IsAlive(Nave nave);
	static void UpdateImmunizationTime(Nave& nave, float deltaTime);

	void Init()
	{
		texture = LoadTexture("res/textures/entities/nave/nave.png");
	}

	void Update(Nave& nave, float deltaTime, bool isAccelerating)
	{
		UpdateRotation(nave);
		AccelerateTowardsMouse(nave, deltaTime, isAccelerating);
		LimitSpeed(nave);
		Move(nave, deltaTime);
		WrapAroundScreen(nave);
		UpdateImmunizationTime(nave, deltaTime);

		nave.isActive = IsAlive(nave);
	}

	void Draw(Nave nave)
	{
		Rectangle source = { 0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height) };
		Rectangle dest = { nave.x, nave.y, nave.radius * 2.0f, nave.radius * 2.0f };
		Vector2 origin = { nave.radius, nave.radius };

		DrawTexturePro(texture, source, dest, origin, nave.rotation, WHITE);
	}

	void Close()
	{
		UnloadTexture(texture);
	}

	Nave Create()
	{
		Nave newNave{};

		newNave.x = static_cast<float>(SCREEN_WIDTH / 2);
		newNave.y = static_cast<float>(SCREEN_HEIGHT / 2);
		newNave.radius = 30.0f;
		newNave.speedMax = DEFAULT_SPEED;
		newNave.acceleration = DEFAULT_SPEED;
		newNave.rotation = 0.0f;
		newNave.velocityX = 0.0f;
		newNave.velocityY = 0.0f;
		newNave.fireRate = DEFAULT_FIRE_RATE;
		newNave.lives = DEFAULT_LIFE;
		newNave.score = 0;
		newNave.isActive = true;
		newNave.isImmune = false;

		return newNave;
	}

	void ResetAll(Nave& nave)
	{
		nave.x = static_cast<float>(SCREEN_WIDTH / 2);
		nave.y = static_cast<float>(SCREEN_HEIGHT / 2);
		nave.rotation = 0.0f;
		nave.speedMax = DEFAULT_SPEED;
		nave.acceleration = DEFAULT_SPEED;
		nave.velocityX = 0.0f;
		nave.velocityY = 0.0f;
		nave.fireRate = DEFAULT_FIRE_RATE;
		nave.lives = DEFAULT_LIFE;
		nave.score = 0;
		nave.isActive = true;
		nave.isImmune = false;
	}

	void ResetLives(Nave& nave)
	{
		nave.lives = DEFAULT_LIFE;
		nave.isActive = true;
	}

	void TakeDamage(Nave& nave)
	{
		nave.lives -= REDUCE_LIFE;
	}

	void AddScore(Nave& nave)
	{
		nave.score += GetIntegerRandomBetween(MIN_SCORE, MAX_SCORE);
	}

	void ApplySpore(SporeType type, Nave& nave)
	{
		switch (type)
		{
		case SporeType::MoreLife:

			if (nave.lives < MAX_LIFE)
			{
				nave.lives += INCREASE_LIFE;
			}

			break;
		case SporeType::MoreShootingSpeed:

			if (nave.fireRate > MAX_FIRE_RATE)
			{
				nave.fireRate -= REDUCE_FIRE_RATE;

				if (nave.fireRate < MAX_FIRE_RATE)
				{
					nave.fireRate = MAX_FIRE_RATE;
				}
			}

			break;
		case SporeType::MoreMovementSpeed:

			if (nave.speedMax < MAX_SPEED && nave.acceleration < MAX_SPEED)
			{
				nave.speedMax += INCREASE_SPEED;
				nave.acceleration += INCREASE_SPEED;
			}

			break;
		case SporeType::ApplyImmunity:

			nave.isImmune = true;

			break;
		default:

			// THERE ARE NO MORE TYPES OF SPORES

			break;
		}
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

	static bool IsAlive(Nave nave)
	{
		if (nave.lives > 0)
		{
			return true;
		}

		return false;
	}

	static void UpdateImmunizationTime(Nave& nave, float deltaTime)
	{
		if (!nave.isImmune)
		{
			return;
		}

		nave.immunityTimer += deltaTime;

		if (nave.immunityTimer >= MAX_IMMUNITY_TIME)
		{
			nave.isImmune = false;
			nave.immunityTimer = 0.0f;
		}
	}
}