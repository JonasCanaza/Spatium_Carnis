#pragma once

#include "raylib.h"

#include <string>

namespace Button
{
    enum class ButtonState
    {
        Normal,
        Hover,
        Pressed
    };

    struct Button
    {
        Rectangle rect;
        Color normalColor;
        Color hoverColor;
        Color pressedColor;
        std::string text;
        ButtonState state;
        bool clicked;
    };

    void Init();
    void Update(Button& button);
    void Draw(Button button);
    Button Create(float x, float y, float width, float height, const std::string& text);
    bool IsMouseOverButton(Button button);
}