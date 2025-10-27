#include "button.h"

#include "raylib.h"

namespace Button
{
	static Texture normal;
	static Texture hover;
	static Texture pressed;

	static bool IsMouseOverButton(Button button);

	void Init() // IMPORTANT: ONLY DO "Init();" IN "game.cpp"
	{
		normal = LoadTexture("res/textures/ui/buttons/normal.png");
		hover = LoadTexture("res/textures/ui/buttons/hover.png");
		pressed = LoadTexture("res/textures/ui/buttons/pressed.png");
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
		Texture texture = normal;

		switch (button.state)
		{
		case ButtonState::Normal:

			texture = normal;

			break;
		case ButtonState::Hover:

			texture = hover;

			break;
		case ButtonState::Pressed:

			texture = pressed;

			break;
		default:

			// NO MORE BUTTON STATES

			break;
		}

		Rectangle source = { 0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height) };
		Rectangle dest = { button.layout.x, button.layout.y, button.layout.width, button.layout.height };
		Vector2 origin = { 0.0f, 0.0f };

		DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);

		int fontSize = 20;
		int textWidth = MeasureText(button.text.c_str(), fontSize);

		DrawText(button.text.c_str(), static_cast<int>(button.layout.x + (button.layout.width - textWidth) / 2), static_cast<int>(button.layout.y + (button.layout.height - fontSize) / 2), fontSize, WHITE);
	}

	void Close() // IMPORTANT: ONLY DO "Close();" IN "game.cpp"
	{
		UnloadTexture(normal);
		UnloadTexture(hover);
		UnloadTexture(pressed);
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