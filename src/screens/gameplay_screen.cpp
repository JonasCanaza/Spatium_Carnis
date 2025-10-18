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
		// TEST!!!
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

		// TEST!!!
		DrawText("Gameplay Screen", 0, 0, 25, WHITE);

		EndDrawing();
	}

	void Close()
	{

	}
}