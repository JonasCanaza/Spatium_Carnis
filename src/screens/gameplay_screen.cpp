#include "screens/gameplay_screen.h"

#include "raylib.h"

#include "entities/nave.h"
#include "game/game.h"

namespace Gameplay
{
	static Nave::Nave nave;

	static float deltaTime;

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
		deltaTime = GetFrameTime();

		Nave::Update(nave, deltaTime);
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