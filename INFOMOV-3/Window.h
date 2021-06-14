#pragma once
#include <cstdio>
#include "SDL.h"
class Window
{
public:
	SDL_Window * window;                    // Declare a pointer

	Window();
	~Window();
};

