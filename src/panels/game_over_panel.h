#pragma once

namespace Nave
{
	struct Nave;
}

namespace GameOverPanel
{
	extern bool isActive;

	void Init();
	void Update();
	void Draw(Nave::Nave nave);
	void Close();
}