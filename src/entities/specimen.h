#pragma once

namespace Specimen
{
	enum class SpawnSide
	{
		None,
		Top,
		Right,
		Bottom,
		Left
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
	void Reset(Specimen& specimen);
}