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
		int lives;
		bool isActive;
	};

	void Init();
	void Input();
	void Update(Nave& nave, float deltaTime);
	void Draw(Nave nave);
	void Close();

	Nave Create();
}