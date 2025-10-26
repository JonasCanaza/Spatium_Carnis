#pragma once

namespace Nave
{
	struct Nave;
}

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
		float rotation;
		float velocityX;
		float velocityY;
		bool isActive;
	};

	void Init();
	void Update(Fungus& fungus, float deltaTime, Nave::Nave nave);
	void Draw(Fungus fungus);
	void Close();

	Fungus Create(float x, float y, float velocityX, float velocityY);
	Fungus SpawnAtSide(SpawnSide side);
	void Reset(Fungus& fungus);
}