#pragma once

namespace PowerUp
{
	struct PowerUP
	{
		float x;
		float y;
		float radius;
		float rotation;
		float velocityX;
		float velocityY;
		bool isActive;
	};

	void Init();
	void Update(PowerUP& powerUP, float deltaTime);
	void Draw(PowerUP powerUP);
	void Close();

	PowerUP Create();
	void Reset(PowerUP& powerUP);
}