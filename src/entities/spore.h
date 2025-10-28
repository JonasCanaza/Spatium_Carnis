#pragma once

namespace Spore
{
	static const float VELOCITY_MIN = 75.0f;
	static const float VELOCITY_MAX = 100.0f;

	enum class SpawnSide
	{
		None,
		Top,
		Right,
		Bottom,
		Left
	};

	struct Spore
	{
		float x;
		float y;
		float radius;
		float rotation;
		float rotationSpeed;
		float velocityX;
		float velocityY;
		float timerLife;
		bool isActive;
	};

	void Init();
	void Update(Spore& spore, float deltaTime);
	void Draw(Spore spore);
	void Close();

	Spore Create(float x, float y, float velocityX, float velocityY);
	Spore SpawnAtSide(SpawnSide side);
	void Reset(Spore& spore);
}