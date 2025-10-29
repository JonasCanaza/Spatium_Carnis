#include "game_over_panel.h"

#include "raylib.h"

#include "interface/button.h"
#include "interface/ui_constants.h"
#include "game/game_constants.h"
#include "game/game.h"
#include "screens/gameplay_screen.h"
#include "sprite/sprite.h"
#include "entities/nave.h"
#include "audio/audio_manager.h"

using namespace UIConstants;

namespace GameOverPanel
{
	bool isActive;

	Sprite::Sprite title;

	static const int MAX_BUTTONS = 3;
	static Button::Button buttons[MAX_BUTTONS];
	static std::string buttonNames[MAX_BUTTONS] = { "Continue", "Restart", "Exit" };

	static Color backgroundColor = { 0, 0, 0, 150 };

	enum ButtonID
	{
		Continue,
		Restart,
		Exit
	};

	static void InitTitle();
	static void InitButtons();
	static void DrawTitle();
	static void DrawButtons();
	static float GetTotalPanelHeight();
	static void DrawScore(Nave::Nave nave);

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

		if (buttons[Continue].clicked)
		{
			isActive = false;
			Gameplay::Continue();
			AudioManager::ResumeMusic(AudioManager::MusicID::MUSIC_GAMEPLAY);
		}

		if (buttons[Restart].clicked)
		{
			isActive = false;
			Gameplay::Reset();
			AudioManager::StopMusic(AudioManager::MusicID::MUSIC_GAMEPLAY);
			AudioManager::PlayMusic(AudioManager::MusicID::MUSIC_GAMEPLAY);
		}

		if (buttons[Exit].clicked)
		{
			isActive = false;
			Gameplay::Reset();
			SpatiumCarnis::currentScene = SpatiumCarnis::Scenes::MainMenu;
			AudioManager::StopMusic(AudioManager::MusicID::MUSIC_GAMEPLAY);
			AudioManager::PlayMusic(AudioManager::MusicID::MUSIC_MENU);
		}
	}

	void Draw(Nave::Nave nave)
	{
		if (!isActive)
		{
			return;
		}

		DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backgroundColor);

		DrawTitle();
		DrawButtons();
		DrawScore(nave);
	}

	void Close()
	{
		UnloadTexture(title.texture);
	}

	static void InitTitle()
	{
		title.texture = LoadTexture("res/textures/ui/titles/game_over.png");

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
			buttons[i] = Button::Create(buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, buttonNames[i], Button::Type::Common);
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

	static void DrawScore(Nave::Nave nave)
	{
		int screenCenterX = SCREEN_WIDTH / 2;

		std::string scoreText = "Score: " + std::to_string(nave.score);

		Vector2 textSize = MeasureTextEx(SpatiumCarnis::font, scoreText.c_str(), UIConstants::USER_INTERFACE_FONT_SIZE, UIConstants::SPACING);

		float scoreX = static_cast<float>(screenCenterX) - textSize.x / 2.0f;
		float scoreY = static_cast<float>(SCREEN_HEIGHT) / 2.0f - BUTTON_HEIGHT / 2.0f;

		DrawTextEx(SpatiumCarnis::font, scoreText.c_str(), { scoreX, scoreY }, UIConstants::USER_INTERFACE_FONT_SIZE, UIConstants::SPACING, WHITE);
	}
}