#pragma once

#include <string>

namespace Button
{
    enum class ButtonState
    {
        Normal,
        Hover,
        Pressed
    };

    struct Layout
    {
        float x;
        float y;
        float width;
        float height;
    };

    struct Button
    {
        ButtonState state = ButtonState::Normal;
        Layout layout;
        std::string text = "No text";
        bool clicked = false;
    };

    void Init();
    void Update(Button& button);
    void Draw(Button button);
    Button Create(float x, float y, float width, float height, std::string text);
}