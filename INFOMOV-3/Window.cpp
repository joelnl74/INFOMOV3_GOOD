#include "Window.h"


Window::Window()
{
	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
										   // Create an application window with the following settings:
	window = SDL_CreateWindow(
		"Software Renderer",               // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		800,                               // width, in pixels
		600,                               // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
	}

	/// <summary>
	/// Create context for OpenCL to use.
	/// </summary>
	SDL_CreateRenderer(window, 1, SDL_WINDOW_OPENGL);

	// The window is open: could enter program loop here (see SDL_PollEvent())
}
Window::~Window()
{
	delete window;
}
