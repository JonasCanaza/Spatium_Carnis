#include "button.h"

#include "raylib.h"

#include "audio/audio_manager.h"

namespace Button
{
	static Texture commonNormal;
	static Texture commonHover;
	static Texture commonPressed;

	static Texture pauseNormal;
	static Texture pauseHover;
	static Texture pausePressed;

	static bool IsMouseOverButton(Button button);
	static Texture GetButtonTexture(Button button);

	void Init() // IMPORTANT: ONLY DO "Init();" IN "game.cpp"
	{
		commonNormal = LoadTexture("res/textures/ui/buttons/common_normal.png");
		commonHover = LoadTexture("res/textures/ui/buttons/common_hover.png");
		commonPressed = LoadTexture("res/textures/ui/buttons/common_pressed.png");

		pauseNormal = LoadTexture("res/textures/ui/buttons/pause_normal.png");
		pauseHover = LoadTexture("res/textures/ui/buttons/pause_hover.png");
		pausePressed = LoadTexture("res/textures/ui/buttons/pause_pressed.png");
	}

	void Update(Button& button)
	{
		button.clicked = false;
		ButtonState previousState = button.state;

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
					AudioManager::PlaySfx(AudioManager::SfxID::SFX_BUTTON_PRESSED);
				}

				button.state = ButtonState::Hover;
			}
		}
		else
		{
			button.state = ButtonState::Normal;
		}

		if (button.state == ButtonState::Hover &&
			previousState != ButtonState::Hover &&
			previousState != ButtonState::Pressed)
		{
			AudioManager::PlaySfx(AudioManager::SFX_BUTTON_HOVER);
		}
	}

	void Draw(Button button)
	{
		Texture texture = GetButtonTexture(button);

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
		UnloadTexture(commonNormal);
		UnloadTexture(commonHover);
		UnloadTexture(commonPressed);

		UnloadTexture(pauseNormal);
		UnloadTexture(pauseHover);
		UnloadTexture(pausePressed);
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

	static Texture GetButtonTexture(Button button)
	{
		switch (button.state)
		{
		case ButtonState::Normal:

			switch (button.type)
			{
			case Type::Common:

				return commonNormal;

				break;
			case Type::Pause:

				return pauseNormal;

				break;
			default:

				// THERE ARE NO MORE TYPES OF BUTTONS

				break;
			}

			break;
		case ButtonState::Hover:

			switch (button.type)
			{
			case Type::Common:

				return commonHover;

				break;
			case Type::Pause:

				return pauseHover;

				break;
			default:

				// THERE ARE NO MORE TYPES OF BUTTONS

				break;
			}

			break;
		case ButtonState::Pressed:

			switch (button.type)
			{
			case Type::Common:

				return commonPressed;

				break;
			case Type::Pause:

				return pausePressed;

				break;
			default:

				// THERE ARE NO MORE TYPES OF BUTTONS

				break;
			}

			break;
		default:

			// NO MORE BUTTON STATES

			break;
		}

		return commonNormal;
	}
}