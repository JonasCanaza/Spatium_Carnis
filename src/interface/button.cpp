#include "button.h"

namespace Button
{
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
		Color color = button.normalColor;

		switch (button.state)
		{
		case ButtonState::Normal:

			color = button.normalColor;

			break;
		case ButtonState::Hover:

			color = button.hoverColor;

			break;
		case ButtonState::Pressed:

			color = button.pressedColor;

			break;
		default:

			// NO MORE BUTTON STATES

			break;
		}

		DrawRectangleRec(button.rect, color);
		DrawRectangleLinesEx(button.rect, 2, WHITE);

		int fontSize = 20;
		int textWidth = MeasureText(button.text.c_str(), fontSize);

		DrawText(button.text.c_str(), static_cast<int>(button.rect.x + (button.rect.width - textWidth) / 2), static_cast<int>(button.rect.y + (button.rect.height - fontSize) / 2), fontSize, WHITE);
	}

	Button Create(float x, float y, float width, float height, const std::string& text)
	{
		Button button;

		button.rect = { x, y, width, height };
		button.normalColor = { 100, 80, 130, 255 };
		button.hoverColor = { 135, 95, 175, 255 };
		button.pressedColor = { 80, 65, 100, 255 };
		button.text = text;
		button.state = ButtonState::Normal;
		button.clicked = false;

		return button;
	}

	bool IsMouseOverButton(Button button)
	{
		Vector2 mouse = GetMousePosition();

		return (mouse.x >= button.rect.x &&
			mouse.x <= button.rect.x + button.rect.width &&
			mouse.y >= button.rect.y &&
			mouse.y <= button.rect.y + button.rect.height);
	}
}