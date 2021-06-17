#include <iostream>
#include <stdio.h>

#include "Window.h"
#include "CLApplication.h"
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

	//create window,draw surface and Rasterizer
	Window *window = new Window();
	SDL_Surface *screen = SDL_GetWindowSurface(window->window);
	CLApplication* clApplication = new CLApplication();
	Rasterizer *rasterizer = new Rasterizer(screen);

	clApplication->InitCL();

	rasterizer->mode = RasterizerMode::Line_And_Fill;
	Mesh bunny1(EruptionMath::vec3(400.0f, 450.0f, 0.0f), "Resources/OBJ/bunny.obj" );
	Mesh bunny2(EruptionMath::vec3(200.0f, 450.0f, 0.0f), "Resources/OBJ/bunny.obj");
	Mesh bunny3(EruptionMath::vec3(600.0f, 450.0f, 0.0f), "Resources/OBJ/bunny.obj");

	Mesh bunny4(EruptionMath::vec3(400.0f, 150.0f, 0.0f), "Resources/OBJ/bunny.obj");
	Mesh bunny5(EruptionMath::vec3(200.0f, 150.0f, 0.0f), "Resources/OBJ/bunny.obj");
	Mesh bunny6(EruptionMath::vec3(600.0f, 150.0f, 0.0f), "Resources/OBJ/bunny.obj");

	EruptionMath::Color color1(0 ,255, 0);
	EruptionMath::Color color2(0, 0, 255);
	EruptionMath::Color color3(255, 0, 0);

	while (g_Running)
	{
		old_time = current_time;
		current_time = SDL_GetTicks();

		ftime = (current_time - old_time) / 1000.0f;
		// printf("%f", ftime);
		SDL_Event event;
		while (SDL_PollEvent(&event))
			HandleEvent(event);

			SDL_FillRect(screen, 0, 0);
			//render here
			bunny1.Draw(*rasterizer, color1, color1, ftime);
			bunny2.Draw(*rasterizer, color2, color2, ftime);
			bunny3.Draw(*rasterizer, color3, color3, ftime);

			bunny4.Draw(*rasterizer, color3, color3, ftime);
			bunny5.Draw(*rasterizer, color2, color2, ftime);
			bunny6.Draw(*rasterizer, color1, color1, ftime);

			SDL_UpdateWindowSurface(window->window);
	}
	// Close and destroy the window
	SDL_DestroyWindow(window->window);

	// Clean up
	SDL_Quit();

	return 0;
}