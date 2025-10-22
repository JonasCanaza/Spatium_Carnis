#pragma once

namespace Nave
{
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
		float reload;
		int lives;
		bool isActive;
	};

	void Init();
	void Update(Nave& nave, float deltaTime, bool isAccelerating);
	void Draw(Nave nave);
	void Close();

	Nave Create();
	void Reset(Nave& nave);
}