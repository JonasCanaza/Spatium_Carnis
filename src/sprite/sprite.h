#pragma once

#include "raylib.h"

namespace Sprite
{
    struct Sprite
    {
        Texture2D texture{};
        Vector2 position{};
        Color tint{};
    };
}