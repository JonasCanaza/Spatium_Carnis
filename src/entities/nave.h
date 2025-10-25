#pragma once

namespace Nave
{
	enum class SporeType
	{
		MoreLife,
		MoreShootingSpeed,
		MoreMovementSpeed,
		ApplyImmunity,
	};

	struct Nave
	{
		float x;
		float y;
		float radius;
		float speedMax;
		float acceleration;
		float rotation;
		float velocityX;
		float velocityY;
		float fireRate;
		float immunityTimer;
		int lives;
		int score;
		bool isActive;
		bool isImmune;
	};

	void Init();
	void Update(Nave& nave, float deltaTime, bool isAccelerating);
	void Draw(Nave nave);
	void Close();

	Nave Create();
	void Reset(Nave& nave);
	void TakeDamage(Nave& nave);
	void AddScore(Nave& nave);
	void ApplySpore(SporeType type, Nave& nave);
}