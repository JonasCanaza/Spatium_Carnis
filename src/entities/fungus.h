#pragma once

namespace Fungus
{
	enum class SpawnSide
	{
		None,
		Top,
		Right,
		Bottom,
		Left
	};

	struct Fungus
	{
		float x;
		float y;
		float radius;
		float speedMax;
		float acceleration;
		float rotation;
		float velocityX;
		float velocityY;
		bool isActive;
	};

	void Init();
	void Update();
	void Draw();
	void Close();
}