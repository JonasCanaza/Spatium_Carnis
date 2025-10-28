#include "screens/how_to_play_screen.h"

#include "raylib.h"

#include "game/game.h"
#include "game/game_constants.h"
#include "interface/button.h"
#include "interface/ui_constants.h"

using namespace UIConstants;

namespace HowToPlay
{
	static Texture descriptionTexture;

	Button::Button button;

	static void InitButton();
	static void UpdateButton();
	static void DrawButton();

	void Init()
	{
		descriptionTexture = LoadTexture("res/textures/descriptions/how_to_play.png");

		InitButton();
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
		UpdateButton();
	}

	void Draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);

		DrawTexture(descriptionTexture, 0, 0, WHITE);
		DrawButton();

		EndDrawing();
	}

	void Close()
	{
		UnloadTexture(descriptionTexture);
	}

	static void InitButton()
	{
		float x = static_cast<float>(SCREEN_WIDTH) / 2.0f - BUTTON_WIDTH / 2.0;
		float buttonMarginBottom = 10.0f;
		float y = static_cast<float>(SCREEN_HEIGHT) - BUTTON_HEIGHT - buttonMarginBottom;

		button = Button::Create(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, "Return to menu", Button::Type::Common);
	}

	static void UpdateButton()
	{
		Button::Update(button);

		if (button.clicked)
		{
			SpatiumCarnis::currentScene = SpatiumCarnis::Scenes::MainMenu;
		}
	}

	static void DrawButton()
	{
		Button::Draw(button);
	}
}