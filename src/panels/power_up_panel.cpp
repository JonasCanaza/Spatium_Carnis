#include "power_up_panel.h"

#include "raylib.h"

#include "game/game_constants.h"

namespace PowerUpPanel
{
	bool isActive;

	void Init()
	{
		isActive = false;
	}

	void Update()
	{

	}

	void Draw()
	{
		if (!isActive)
		{
			return;
		}

		Color backgroundColor = { 0, 0, 0, 100 };
		DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backgroundColor);
	}

	void Close()
	{

	}
}