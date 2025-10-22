#include "exit_panel.h"

#include "raylib.h"

#include "utilities/constants.h"

namespace ExitPanel
{
	bool isActive;

	void Init()
	{

	}

	void Update()
	{

	}

	void Draw()
	{
		Color backgroundColor = { 0, 0, 0, 100 };
		DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backgroundColor);
	}

	void Close()
	{

	}
}