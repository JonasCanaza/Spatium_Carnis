#include "pause_panel.h"

#include "raylib.h"

#include "interface/button.h"
#include "interface/ui_constants.h"
#include "game/game_constants.h"
#include "game/game.h"
#include "screens/gameplay_screen.h"

using namespace UIConstants;

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

	// TEST TITLE!!!
	static Rectangle title = { 0.0f, 0.0f, TITLE_WIDTH, TITLE_HEIGHT };

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
			isActive = false;
			Gameplay::Reset();
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

		DrawTitle();
		DrawButtons();
	}

	void Close()
	{

	}

	static void InitTitle()
	{
		const float totalPanelHeight = GetTotalPanelHeight();
		const float titleStartY = (SCREEN_HEIGHT - totalPanelHeight) / 2.0f;

		title.x = (SCREEN_WIDTH - TITLE_WIDTH) / 2.0f;
		title.y = titleStartY;
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
		int x = static_cast<int>(title.x);
		int y = static_cast<int>(title.y);
		int width = static_cast<int>(title.width);
		int height = static_cast<int>(title.height);

		DrawRectangle(x, y, width, height, WHITE);
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