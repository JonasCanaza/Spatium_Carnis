#include "button.h"

#include "raylib.h"

namespace Button
{
	// TEST!!!

	static Color normalColor = { 100, 80, 130, 255 };
	static Color hoverColor = { 135, 95, 175, 255 };
	static Color pressedColor = { 80, 65, 100, 255 };

	// ================

	static bool IsMouseOverButton(Button button);

	void Init()
	{

	}

	void Update(Button& button)
	{
		button.clicked = false;

		if (IsMouseOverButton(button))
		{
			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
			{
				button.state = ButtonState::Pressed;
			}
			else
			{
				if (button.state == ButtonState::Pressed)
				{
					button.clicked = true;
				}

				button.state = ButtonState::Hover;
			}
		}
		else
		{
			button.state = ButtonState::Normal;
		}
	}

	void Draw(Button button)
	{
		Color color = normalColor;

		switch (button.state)
		{
		case ButtonState::Normal:

			color = normalColor;

			break;
		case ButtonState::Hover:

			color = hoverColor;

			break;
		case ButtonState::Pressed:

			color = pressedColor;

			break;
		default:

			// NO MORE BUTTON STATES

			break;
		}

		Rectangle rect = { button.layout.x, button.layout.y, button.layout.width, button.layout.height };

		DrawRectangleRec(rect, color);
		DrawRectangleLinesEx(rect, 2, WHITE);

		int fontSize = 20;
		int textWidth = MeasureText(button.text.c_str(), fontSize);

		DrawText(button.text.c_str(), static_cast<int>(button.layout.x + (button.layout.width - textWidth) / 2), static_cast<int>(button.layout.y + (button.layout.height - fontSize) / 2), fontSize, WHITE);
	}

	Button Create(float x, float y, float width, float height, std::string text)
	{
		Button button;

		button.layout = { x, y, width, height };
		button.text = text;
		button.state = ButtonState::Normal;
		button.clicked = false;

		return button;
	}

	static bool IsMouseOverButton(Button button)
	{
		Vector2 mouse = GetMousePosition();

		return (mouse.x >= button.layout.x &&
			mouse.x <= button.layout.x + button.layout.width &&
			mouse.y >= button.layout.y &&
			mouse.y <= button.layout.y + button.layout.height);
	}
}