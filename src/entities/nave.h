#pragma once

namespace Nave
{
	struct Nave
	{
		float x;
		float y;
		float radius;
		float speed;
		float acceleration;
		float rotation;
		float velocityX;
		float velocityY;
		int lives;
		bool active;
	};

	void Init();
	void Input();
	void Update(Nave& nave);
	void Draw(Nave nave);
	void Close();

	Nave Create();
}