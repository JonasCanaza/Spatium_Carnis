#include "game.h"

#include "raylib.h"

#include "screens/main_menu_screen.h"
#include "utilities/constants.h"

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
		InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, PROGRAM_NAME.c_str());
		Init();

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
		MainMenu::Init();
	}

	static void Input()
	{
		switch (currentScene)
		{
		case SpatiumCarnis::Scenes::MainMenu:

			MainMenu::Input();

			break;
		case SpatiumCarnis::Scenes::Gameplay:

			

			break;
		case SpatiumCarnis::Scenes::HowToPlay:

			

			break;
		case SpatiumCarnis::Scenes::Credits:

			

			break;
		default:

			// THERE ARE NO OTHER SCENES

			break;
		}
	}

	static void Update()
	{
		switch (currentScene)
		{
		case SpatiumCarnis::Scenes::MainMenu:

			MainMenu::Update();

			break;
		case SpatiumCarnis::Scenes::Gameplay:



			break;
		case SpatiumCarnis::Scenes::HowToPlay:



			break;
		case SpatiumCarnis::Scenes::Credits:



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



			break;
		case SpatiumCarnis::Scenes::HowToPlay:



			break;
		case SpatiumCarnis::Scenes::Credits:



			break;
		default:

			// THERE ARE NO OTHER SCENES

			break;
		}
	}

	static void Close()
	{
		MainMenu::Close();
	}
}