#pragma once

namespace Spore
{
	struct Spore
	{
		float x;
		float y;
		float radius;
		float rotation;
		float velocityX;
		float velocityY;
		float timerLife;
		bool isActive;
	};

	void Init();
	void Update(Spore& spore, float deltaTime);
	void Draw(Spore spore);
	void Close();

	Spore Create();
	void Reset(Spore& spore);
}