#include "main_menu_screen.h"

#include <iostream>

#include "raylib.h"

#include "game/game.h"
#include "interface/button.h"
#include "utilities/constants.h"

namespace MainMenu
{
	static const int MAX_BUTTONS = 4;
	static Button::Button buttons[MAX_BUTTONS];
	static std::string buttonNames[MAX_BUTTONS] = { "Play", "How To Play", "Credits", "Exit" };

	enum ButtonID
	{
		Play,
		HowToPlay,
		Credits,
		Exit,
	};

	// TEST LOGO!!!

	static const float LOGO_WIDTH = 500.0f;
	static const float LOGO_HEIGHT = 300.0f;
	static const float BUTTON_WIDTH = 350.0f;
	static const float BUTTON_HEIGHT = 50.0f;
	static const float BUTTON_MARGIN_Y = 10.0f;
	static const float LOGO_TO_BUTTONS_MARGIN = 40.0f;

	static Rectangle logo = { 0.0f, 0.0f, LOGO_WIDTH, LOGO_HEIGHT };

	// ===============

	static void InitLogo();
	static void InitButtons();
	static void DrawLogo();
	static void DrawButtons();
	static float GetTotalMenuHeight();

	void Init()
	{
		InitLogo();
		InitButtons();
	}

	void Input()
	{

	}

	void Update()
	{
		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			Button::Update(buttons[i]);
		}

		if (buttons[Play].clicked)
		{
			SpatiumCarnis::currentScene = SpatiumCarnis::Scenes::Gameplay;
		}

		if (buttons[HowToPlay].clicked)
		{
			SpatiumCarnis::currentScene = SpatiumCarnis::Scenes::HowToPlay;
		}

		if (buttons[Credits].clicked)
		{
			SpatiumCarnis::currentScene = SpatiumCarnis::Scenes::Credits;
		}

		if (buttons[Exit].clicked)
		{
			SpatiumCarnis::isRunning = false;
		}
	}

	void Draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);

		DrawLogo();
		DrawButtons();

		EndDrawing();
	}

	void Close()
	{

	}

	static void InitLogo()
	{
		const float totalMenuHeight = GetTotalMenuHeight();
		const float logoStartY = (SCREEN_HEIGHT - totalMenuHeight) / 2.0f;

		logo.x = 0.0f;
		logo.y = logoStartY;
	}

	static void InitButtons()
	{
		const float totalMenuHeight = GetTotalMenuHeight();
		const float logoStartY = (SCREEN_HEIGHT - totalMenuHeight) / 2.0f;
		const float buttonsStartY = logoStartY + LOGO_HEIGHT + LOGO_TO_BUTTONS_MARGIN;

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			const float buttonX = 0.0f;
			const float buttonY = buttonsStartY + (BUTTON_HEIGHT + BUTTON_MARGIN_Y) * i;

			buttons[i] = Button::Create(buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, buttonNames[i]);
		}
	}

	static void DrawLogo()
	{
		int x = static_cast<int>(logo.x);
		int y = static_cast<int>(logo.y);
		int width = static_cast<int>(logo.width);
		int height = static_cast<int>(logo.height);

		DrawRectangle(x, y, width, height, WHITE);
	}
	
	static void DrawButtons()
	{
		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			Button::Draw(buttons[i]);
		}
	}

	static float GetTotalMenuHeight()
	{
		return LOGO_HEIGHT + LOGO_TO_BUTTONS_MARGIN + (BUTTON_HEIGHT * MAX_BUTTONS) + (BUTTON_MARGIN_Y * (MAX_BUTTONS - 1));
	}
}