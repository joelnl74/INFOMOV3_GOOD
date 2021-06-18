#include <iostream>
#include <stdio.h>

#include "Window.h"
#include "Rasterizer.h"
#include "Mesh.h"
#include "BasicShader.h"

static unsigned int ScreenWidth = 800;
static unsigned int ScreenHeigth = 600;

static bool g_Running = true;

static void HandleKeyEvent(const SDL_Event &event)
{
	switch (event.key.keysym.sym) {
	default:
		break;
	case SDLK_ESCAPE:
		g_Running = false;
		break;
	}
}
static void HandleEvent(const SDL_Event &event)
{
	switch (event.type) {
	default:
		break;
	case SDL_QUIT:
		g_Running = false;
		break;
	case SDL_KEYDOWN:
		HandleKeyEvent(event);
		break;
	}
}
int main(int argc, char* argv[])
{
	// Timing variables
	unsigned int old_time, current_time = 0;
	float ftime;

	clock_t current_ticks, delta_ticks;
	clock_t fps = 0;

	//create window,draw surface and Rasterizer
	Window *window = new Window();
	SDL_Surface *screen = SDL_GetWindowSurface(window->window);
	Rasterizer *rasterizer = new Rasterizer(screen);

	rasterizer->mode = RasterizerMode::Line_And_Fill;
	Mesh mesh(EruptionMath::vec3(400.0f, 450.0f, 0.0f), "Resources/OBJ/bunny.obj" );

	EruptionMath::Color color(255 ,255, 255);
	//red pixel
	EruptionMath::Color color2(0, 0, 255);

	while (g_Running)
	{
		old_time = current_time;
		current_time = SDL_GetTicks();

		ftime = (current_time - old_time) / 1000.0f;

		// FPS
		current_ticks = clock();
		// END FPS

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			HandleEvent(event);

			SDL_FillRect(screen, 0, 0);
			//render here
			mesh.Draw(*rasterizer, color, ftime);
			// bunny2.Draw(*rasterizer, color2, color2, ftime);
			// bunny3.Draw(*rasterizer, color3, color3, ftime);

			// bunny4.Draw(*rasterizer, color3, color3, ftime);
			// bunny5.Draw(*rasterizer, color2, color2, ftime);
			// bunny6.Draw(*rasterizer, color1, color1, ftime);

			SDL_UpdateWindowSurface(window->window);
		}


		delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene
		if (delta_ticks > 0)
			fps = CLOCKS_PER_SEC / delta_ticks;
		std::cout << "FPS: " << fps << std::endl;
	}
	// Close and destroy the window
	SDL_DestroyWindow(window->window);

	// Clean up
	SDL_Quit();

	return 0;
}
