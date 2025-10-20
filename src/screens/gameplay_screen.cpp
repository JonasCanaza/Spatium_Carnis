#include "screens/gameplay_screen.h"

#include "raylib.h"

#include "entities/nave.h"
#include "game/game.h"

namespace Gameplay
{
	static Nave::Nave nave;

	void Init()
	{
		Nave::Init();

		nave = Nave::Create();
	}

	void Input()
	{
		if (IsKeyPressed(KEY_ESCAPE))
		{
			SpatiumCarnis::currentScene = SpatiumCarnis::Scenes::MainMenu;
		}

		Nave::Input();
	}

	void Update()
	{
		Nave::Update(nave);
	}

	void Draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);

		Nave::Draw(nave);

		EndDrawing();
	}

	void Close()
	{
		Nave::Close();
	}
}