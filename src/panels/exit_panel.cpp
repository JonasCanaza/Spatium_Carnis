#include "exit_panel.h"

#include "raylib.h"

#include "interface/button.h"
#include "interface/ui_constants.h"
#include "game/game_constants.h"
#include "game/game.h"
#include "screens/gameplay_screen.h"
#include "sprite/sprite.h"

using namespace UIConstants;

namespace ExitPanel
{
	bool isActive;

	Sprite::Sprite title;

	static const int MAX_BUTTONS = 2;
	static Button::Button buttons[MAX_BUTTONS];
	static std::string buttonNames[MAX_BUTTONS] = { "Return", "Exit" };

	enum ButtonID
	{
		Return,
		Exit
	};

	static void InitTitle();
	static void InitButtons();
	static void DrawTitle();
	static void DrawButtons();
	static float GetTotalPanelHeight();

	void Init()
	{
		isActive = false;

		InitTitle();
		InitButtons();
	}

	void Update()
	{
		if (!isActive)
		{
			return;
		}

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			Button::Update(buttons[i]);
		}

		if (buttons[Return].clicked)
		{
			isActive = false;
		}

		if (buttons[Exit].clicked)
		{
			SpatiumCarnis::isRunning = false;
		}
	}

	void Draw()
	{
		if (!isActive)
		{
			return;
		}

		Color backgroundColor = { 0, 0, 0, 100 };
		DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backgroundColor);

		DrawTitle();
		DrawButtons();
	}

	void Close()
	{
		UnloadTexture(title.texture);
	}

	static void InitTitle()
	{
		title.texture = LoadTexture("res/textures/ui/titles/exit_game.png");

		const float totalPanelHeight = GetTotalPanelHeight();
		const float titleStartY = (SCREEN_HEIGHT - totalPanelHeight) / 2.0f;

		title.position.x = (SCREEN_WIDTH - TITLE_WIDTH) / 2.0f;
		title.position.y = titleStartY;
		title.tint = WHITE;
	}

	static void InitButtons()
	{
		const float totalPanelHeight = GetTotalPanelHeight();
		const float titleStartY = (SCREEN_HEIGHT - totalPanelHeight) / 2.0f;
		const float buttonsStartY = titleStartY + TITLE_HEIGHT + TITLE_TO_BUTTONS_MARGIN;

		const float buttonX = (SCREEN_WIDTH - BUTTON_WIDTH) / 2.0f;

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			const float buttonY = buttonsStartY + (BUTTON_HEIGHT + BUTTON_MARGIN_Y) * i;
			buttons[i] = Button::Create(buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, buttonNames[i]);
		}
	}

	static void DrawTitle()
	{
		int x = static_cast<int>(title.position.x);
		int y = static_cast<int>(title.position.y);

		DrawTexture(title.texture, x, y, title.tint);
	}

	static void DrawButtons()
	{
		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			Button::Draw(buttons[i]);
		}
	}

	static float GetTotalPanelHeight()
	{
		return TITLE_HEIGHT + TITLE_TO_BUTTONS_MARGIN + (BUTTON_HEIGHT * MAX_BUTTONS) + (BUTTON_MARGIN_Y * (MAX_BUTTONS - 1));
	}
}