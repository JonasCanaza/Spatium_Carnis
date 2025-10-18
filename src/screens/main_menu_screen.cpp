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

	static void InitButtons();

	void Init()
	{
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
			std::cout << "Play!" << std::endl;
		}

		if (buttons[HowToPlay].clicked)
		{
			std::cout << "How to play!" << std::endl;
		}

		if (buttons[Credits].clicked)
		{
			std::cout << "Credits!" << std::endl;
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

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			Button::Draw(buttons[i]);
		}

		EndDrawing();
	}

	void Close()
	{

	}

	static void InitButtons()
	{
		float btnWidth = 300.0f;
		float btnHeight = 50.0f;
		float marginBetween = 10.0f;
		float marginTop = 300.0f;

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			float x = 0.0f;
			float y = marginTop + (btnHeight + marginBetween) * i;

			buttons[i] = Button::Create(x, y, btnWidth, btnHeight, buttonNames[i]);
		}
	}
}