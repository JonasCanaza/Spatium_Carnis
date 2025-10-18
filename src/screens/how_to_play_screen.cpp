#include "screens/how_to_play_screen.h"

#include "raylib.h"

#include "game/game.h"

namespace HowToPlay
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
		DrawText("How To Play Screen", 0, 0, 25, WHITE);

		EndDrawing();
	}

	void Close()
	{

	}
}