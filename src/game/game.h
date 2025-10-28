#pragma once

#include "raylib.h"

namespace SpatiumCarnis
{
	enum class Scenes
	{
		MainMenu,
		Gameplay,
		HowToPlay,
		Credits
	};

	extern Scenes currentScene;
	extern bool isRunning;

	extern Font font;

	void Play();
}