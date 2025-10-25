#pragma once

namespace Nave
{
	struct Nave;
}

namespace SporePanel
{
	extern bool isActive;

	void Init();
	void Update(Nave::Nave& nave);
	void Draw();
	void Close();
}