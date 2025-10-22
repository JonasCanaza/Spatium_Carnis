#include "pause_panel.h"

#include "raylib.h"

#include "interface/button.h"
#include "utilities/constants.h"
#include "game/game.h"

namespace PausePanel
{
	bool isActive;

	static const int MAX_BUTTONS = 3;
	static Button::Button buttons[MAX_BUTTONS];
	static std::string buttonNames[MAX_BUTTONS] = { "Resume", "Restart", "Exit" };

	enum ButtonID
	{
		Resume,
		Restart,
		Exit
	};

	static const float BUTTON_WIDTH = 350.0f;
	static const float BUTTON_HEIGHT = 50.0f;
	static const float BUTTON_MARGIN_Y = 10.0f;

	void Init()
	{
		isActive = false;

		float totalHeight = (MAX_BUTTONS * BUTTON_HEIGHT) + ((MAX_BUTTONS - 1) * BUTTON_MARGIN_Y);

		float startY = (SCREEN_HEIGHT - totalHeight) / 2.0f;
		float centerX = SCREEN_WIDTH / 2.0f;

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			float posY = startY + i * (BUTTON_HEIGHT + BUTTON_MARGIN_Y);
			float posX = centerX - (BUTTON_WIDTH / 2.0f);

			buttons[i] = Button::Create(posX, posY, BUTTON_WIDTH, BUTTON_HEIGHT, buttonNames[i]);
		}
	}

	void Update()
	{
		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			Button::Update(buttons[i]);
		}

		if (buttons[Resume].clicked)
		{
			isActive = false;
		}

		if (buttons[Restart].clicked)
		{
			// RESTART LEVEL
		}

		if (buttons[Exit].clicked)
		{
			isActive = false;
			SpatiumCarnis::currentScene = SpatiumCarnis::Scenes::MainMenu;
		}
	}

	void Draw()
	{
		Color backgroundColor = { 0, 0, 0, 100 };

		DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backgroundColor);

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			Button::Draw(buttons[i]);
		}
	}

	void Close()
	{

	}
}