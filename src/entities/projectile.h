#pragma once

namespace Nave
{
	struct Nave;
}

namespace Projectile
{
	struct Projectile
	{
		float x;
		float y;
		float radius;
		float speedMax;
		float rotation;
		float rotationSpeed;
		float velocityX;
		float velocityY;
		bool isActive;
	};

	void Init();
	void Update(Projectile& projectile, float deltaTime);
	void Draw(Projectile projectile);
	void Close();

	Projectile Create(Nave::Nave& nave);
	void Reset(Projectile& Projectile);
}