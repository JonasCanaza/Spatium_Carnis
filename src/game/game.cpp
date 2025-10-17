#include "game.h"

#include "raylib.h"

#include <iostream>

namespace SpatiumCarnis
{
	static void Init();
	static void Input();
	static void Update();
	static void Draw();
	static void Close();

	void Play()
	{
		const int SCREEN_WIDTH = 1024;
		const int SCREEN_HEIGHT = 768;

		Scenes currentScene = Scenes::MainMenu;
		bool isRunning = true;

		InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Spatium Carnis");
		Init();

		while (!WindowShouldClose())
		{
			Input();
			Update();
			Draw();
		}

		Close();
	}

	static void Init()
	{
		std::cout << "Game Init!" << std::endl;
	}

	static void Input()
	{
		std::cout << "Game Input!" << std::endl;
	}

	static void Update()
	{
		std::cout << "Game Update!" << std::endl;
	}

	static void Draw()
	{
		std::cout << "Game Draw!" << std::endl;

		BeginDrawing();

		ClearBackground(BLACK);

		DrawText("Spatium Carnis", 0, 0, 20, WHITE);

		EndDrawing();
	}

	static void Close()
	{
		std::cout << "Game Close!" << std::endl;
	}
}