#pragma once

namespace Specimen
{
	enum class SpawnSide
	{
		Left,
		Right,
		Top,
		Bottom
	};

	enum class Type
	{
		None,
		Big,
		Medium,
		Small
	};

	struct Specimen
	{
		float x;
		float y;
		float radius;
		float rotation;
		float velocityX;
		float velocityY;
		bool isActive;
		Type type;
	};

	void Init();
	void Update(Specimen& specimen, float deltaTime);
	void Draw(Specimen specimen);
	void Close();

	Specimen Create(float x, float y, float velocityX, float velocityY, Type type);
	Specimen SpawnAtSide(SpawnSide side, Type type);
}