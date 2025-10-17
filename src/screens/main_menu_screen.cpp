#include "main_menu_screen.h"

#include "interface/button.h"

#include "raylib.h"

#include <iostream>

namespace MainMenu
{
	static Button::Button testButton;

	void Init()
	{
		// TEST!!!
		testButton = Button::Create(0.0f, 0.0f, 250.0f, 75.0f, "Test Button");
	}

	void Input()
	{

	}

	void Update()
	{
		Button::Update(testButton);

		if (testButton.clicked)
		{
			std::cout << "Click!" << std::endl;
		}
	}

	void Draw()
	{
		BeginDrawing();

		ClearBackground(BLACK);

		DrawText("Main menu", 0, 100, 20, WHITE);

		Button::Draw(testButton);

		EndDrawing();
	}

	void Close()
	{

	}
}