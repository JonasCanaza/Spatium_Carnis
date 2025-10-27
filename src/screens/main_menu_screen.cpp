#include "main_menu_screen.h"

#include "raylib.h"

#include "sprite/sprite.h"
#include "game/game.h"
#include "interface/button.h"
#include "interface/ui_constants.h"
#include "game/game_constants.h"
#include "panels/exit_panel.h"
#include "audio/audio_manager.h"

using namespace UIConstants;

namespace MainMenu
{
	static const Color BACKGROUND_OVERLAY_COLOR = { 0, 0, 0, 60 };
	static Sprite::Sprite background{};

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
	static Rectangle logo = { 0.0f, 0.0f, LOGO_WIDTH, LOGO_HEIGHT };

	static void InitBackground();
	static void InitLogo();
	static void InitButtons();
	static void DrawBackground();
	static void DrawLogo();
	static void DrawButtons();
	static void UpdateButtons();
	static float GetTotalMenuHeight();

	void Init()
	{
		InitBackground();
		InitLogo();
		InitButtons();

		ExitPanel::Init();
	}

	void Input()
	{
		if (IsKeyPressed(KEY_ESCAPE))
		{
			ExitPanel::isActive = !ExitPanel::isActive;
		}
	}

	void Update()
	{
		if (!ExitPanel::isActive)
		{
			UpdateButtons();
		}

		ExitPanel::Update();
	}

	void Draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);

		DrawBackground();
		DrawLogo();
		DrawButtons();

		ExitPanel::Draw();

		EndDrawing();
	}

	void Close()
	{
		ExitPanel::Close();
	}

	static void InitBackground()
	{
		background.texture = LoadTexture("res/textures/backgrounds/menu.png");
		background.position = { 0.0f, 0.0f };
		background.tint = WHITE;
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

	static void DrawBackground()
	{
		int x = static_cast<int>(background.position.x);
		int y = static_cast<int>(background.position.y);

		DrawTexture(background.texture, x, y, background.tint);
		DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BACKGROUND_OVERLAY_COLOR);
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

	static void UpdateButtons()
	{
		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			Button::Update(buttons[i]);
		}

		if (buttons[Play].clicked)
		{
			AudioManager::StopMusic(AudioManager::MusicID::MUSIC_MENU);
			AudioManager::PlayMusic(AudioManager::MusicID::MUSIC_GAMEPLAY);
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
			ExitPanel::isActive = true;
		}
	}

	static float GetTotalMenuHeight()
	{
		return LOGO_HEIGHT + LOGO_TO_BUTTONS_MARGIN + (BUTTON_HEIGHT * MAX_BUTTONS) + (BUTTON_MARGIN_Y * (MAX_BUTTONS - 1));
	}
}