#include "game.h"

#include <ctime>

#include "raylib.h"

#include "screens/main_menu_screen.h"
#include "screens/gameplay_screen.h"
#include "screens/how_to_play_screen.h"
#include "screens/credits_screen.h"
#include "game/game_constants.h"
#include "audio/audio_manager.h"

namespace SpatiumCarnis
{
	Scenes currentScene = Scenes::MainMenu;
	bool isRunning = true;

	static void Init();
	static void Input();
	static void Update();
	static void Draw();
	static void Close();

	void Play()
	{
		srand(static_cast<unsigned int>(time(0)));

		InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, PROGRAM_NAME.c_str());
		Init();
		SetExitKey(KEY_NULL);

		AudioManager::PlayMusic(AudioManager::MusicID::MUSIC_MENU);

		while (!WindowShouldClose() && isRunning)
		{
			Input();
			Update();
			Draw();
		}

		Close();
	}

	static void Init()
	{
		AudioManager::Init();

		MainMenu::Init();
		Gameplay::Init();
		HowToPlay::Init();
		Credits::Init();
	}

	static void Input()
	{
		switch (currentScene)
		{
		case SpatiumCarnis::Scenes::MainMenu:

			MainMenu::Input();

			break;
		case SpatiumCarnis::Scenes::Gameplay:

			Gameplay::Input();

			break;
		case SpatiumCarnis::Scenes::HowToPlay:

			HowToPlay::Input();

			break;
		case SpatiumCarnis::Scenes::Credits:

			Credits::Input();

			break;
		default:

			// THERE ARE NO OTHER SCENES

			break;
		}
	}

	static void Update()
	{
		AudioManager::Update();

		switch (currentScene)
		{
		case SpatiumCarnis::Scenes::MainMenu:

			MainMenu::Update();

			break;
		case SpatiumCarnis::Scenes::Gameplay:

			Gameplay::Update();

			break;
		case SpatiumCarnis::Scenes::HowToPlay:

			HowToPlay::Update();

			break;
		case SpatiumCarnis::Scenes::Credits:

			Credits::Update();

			break;
		default:

			// THERE ARE NO OTHER SCENES

			break;
		}
	}

	static void Draw()
	{
		switch (currentScene)
		{
		case SpatiumCarnis::Scenes::MainMenu:

			MainMenu::Draw();

			break;
		case SpatiumCarnis::Scenes::Gameplay:

			Gameplay::Draw();

			break;
		case SpatiumCarnis::Scenes::HowToPlay:

			HowToPlay::Draw();

			break;
		case SpatiumCarnis::Scenes::Credits:

			Credits::Draw();

			break;
		default:

			// THERE ARE NO OTHER SCENES

			break;
		}
	}

	static void Close()
	{
		AudioManager::Close();

		MainMenu::Close();
		Gameplay::Close();
		HowToPlay::Close();
		Credits::Close();
	}
}