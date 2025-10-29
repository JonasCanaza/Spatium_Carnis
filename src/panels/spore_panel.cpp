#include "spore_panel.h"

#include <iostream>

#include "raylib.h"

#include "interface/button.h"
#include "interface/ui_constants.h"
#include "game/game_constants.h"
#include "entities/nave.h"
#include "sprite/sprite.h"
#include "audio/audio_manager.h"

using namespace UIConstants;

namespace SporePanel
{
	bool isActive;

	Sprite::Sprite title;

	static const int MAX_BUTTONS = 5;
	static Button::Button buttons[MAX_BUTTONS];
	static std::string buttonNames[MAX_BUTTONS] = { "+Life", "+Shooting speed", "+Movement speed", "Immunity", "Cancel" };

	static Color backgroundColor = { 0, 0, 0, 150 };

	enum ButtonID
	{
		Life,
		ShootingSpeed,
		MovementSpeed,
		Immunity,
		Cancel
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

	void Update(Nave::Nave& nave)
	{
		if (!isActive)
		{
			return;
		}

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			Button::Update(buttons[i]);
		}

		if (buttons[Life].clicked)
		{
			Nave::ApplySpore(Nave::SporeType::MoreLife, nave);
			isActive = false;
			AudioManager::PlayMusic(AudioManager::MusicID::MUSIC_GAMEPLAY);
		}

		if (buttons[ShootingSpeed].clicked)
		{
			Nave::ApplySpore(Nave::SporeType::MoreShootingSpeed, nave);
			isActive = false;
			AudioManager::PlayMusic(AudioManager::MusicID::MUSIC_GAMEPLAY);
		}

		if (buttons[MovementSpeed].clicked)
		{
			Nave::ApplySpore(Nave::SporeType::MoreMovementSpeed, nave);
			isActive = false;
			AudioManager::PlayMusic(AudioManager::MusicID::MUSIC_GAMEPLAY);
		}

		if (buttons[Immunity].clicked)
		{
			Nave::ApplySpore(Nave::SporeType::ApplyImmunity, nave);
			isActive = false;
			AudioManager::PlayMusic(AudioManager::MusicID::MUSIC_GAMEPLAY);
		}

		if (buttons[Cancel].clicked)
		{
			isActive = false;
			AudioManager::PlayMusic(AudioManager::MusicID::MUSIC_GAMEPLAY);
		}
	}

	void Draw()
	{
		if (!isActive)
		{
			return;
		}

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
		title.texture = LoadTexture("res/textures/ui/titles/select_spore.png");

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
}