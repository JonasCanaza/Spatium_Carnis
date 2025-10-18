#include "screens/gameplay_screen.h"

#include "raylib.h"

#include "game/game.h"

namespace Gameplay
{
	void Init()
	{

	}

	void Input()
	{
		if (IsKeyPressed(KEY_ESCAPE))
		{
			SpatiumCarnis::currentScene = SpatiumCarnis::Scenes::MainMenu;
		}
	}

	void Update()
	{

	}

	void Draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);

		EndDrawing();
	}

	void Close()
	{

	}
}