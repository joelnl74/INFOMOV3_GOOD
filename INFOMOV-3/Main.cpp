#include <iostream>
#include <stdio.h>

#include "Window.h"
#include "CLApplication.h"
#include "Rasterizer.h"
#include "Mesh.h"

// Threading related libs
#include "precomp.h"

constexpr auto THREADCOUNT = 8;

// Timing variables
unsigned int old_time, current_time = 0;
float ftime;
float total = 0;
int counter = 0;
clock_t current_ticks, delta_ticks;
clock_t fps = 0;

// Application setup
Window* window = new Window();
SDL_Surface* screen = SDL_GetWindowSurface(window->window);
CLApplication* clApplication = new CLApplication();
Rasterizer* rasterizer = new Rasterizer(screen);
static std::vector<Mesh> meshes;
static bool g_Running = true;

EruptionMath::Color color1(0, 255, 0);
EruptionMath::Color color2(0, 0, 255);
EruptionMath::Color color3(255, 0, 0);

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

// Multi Threading variables
HANDLE goSignal[THREADCOUNT], doneSignal[THREADCOUNT];
volatile LONG remaining = 0;
static HANDLE worker[THREADCOUNT];

unsigned long __stdcall workerthread(LPVOID param)
{
	int threadId = *(int*)param;
	SetThreadAffinityMask(worker[threadId], 1 << threadId);
	while (1)
	{
		WaitForSingleObject(goSignal[threadId], INFINITE);
		while (1)
		{
			int task = (int)InterlockedDecrement(&remaining);
			if (task < 0) break;

			SDL_Event event;
			while (SDL_PollEvent(&event))
				HandleEvent(event);

			meshes.at(task).Draw(*rasterizer, color1, color2, ftime);
			/*switch (task) {
			case 0:
				meshes.at(0).Draw(*rasterizer, color1, color2, ftime);
				break;
			case 1:
				meshes.at(1).Draw(*rasterizer, color1, color2, ftime);
				break;
			case 2:
				meshes.at(2).Draw(*rasterizer, color1, color2, ftime);
				break;
			case 3:
				meshes.at(3).Draw(*rasterizer, color1, color2, ftime);;
				break;
			case 4:
				meshes.at(4).Draw(*rasterizer, color1, color2, ftime);;
				break;
			case 5:
				meshes.at(5).Draw(*rasterizer, color1, color2, ftime);;
				break;
			}*/
		}
		SetEvent(doneSignal[threadId]);
	}
}

int main(int argc, char* argv[])
{
	//clApplication->InitCL();

	rasterizer->mode = RasterizerMode::Line_And_Fill;
	Mesh bunny1(EruptionMath::vec3(400.0f, 450.0f, 0.0f), "Resources/OBJ/bunny.obj" );
	Mesh bunny2(EruptionMath::vec3(200.0f, 450.0f, 0.0f), "Resources/OBJ/bunny.obj");
	Mesh bunny3(EruptionMath::vec3(600.0f, 450.0f, 0.0f), "Resources/OBJ/bunny.obj");

	Mesh bunny4(EruptionMath::vec3(400.0f, 150.0f, 0.0f), "Resources/OBJ/bunny.obj");
	Mesh bunny5(EruptionMath::vec3(200.0f, 150.0f, 0.0f), "Resources/OBJ/bunny.obj");
	Mesh bunny6(EruptionMath::vec3(600.0f, 150.0f, 0.0f), "Resources/OBJ/bunny.obj");

	meshes.push_back(bunny1);
	meshes.push_back(bunny2);
	meshes.push_back(bunny3);
	meshes.push_back(bunny4);
	meshes.push_back(bunny5);
	meshes.push_back(bunny6);

	// Worker threads
	static DWORD threadId[THREADCOUNT];
	static int params[THREADCOUNT];
	static bool initialized = false;

	// spawn worker threads
	if (!initialized)
	{
		for (int i = 0; i < THREADCOUNT; i++)
			params[i] = i,
			worker[i] = CreateThread(NULL, 0, workerthread, &params[i], 0, &threadId[i]),
			goSignal[i] = CreateEvent(0, 0, 0, 0),
			doneSignal[i] = CreateEvent(0, 0, 0, 0);
		initialized = true;
	}

#if 1
		while (g_Running)
		{
			if (counter == 100)
				total = counter = 0;

			old_time = current_time;
			current_time = SDL_GetTicks();
			ftime = (current_time - old_time) / 1000.0f;

			// FPS
			current_ticks = clock();
			// END FPS

			remaining = meshes.size(); // Should replace with tiles
			for (int i = 0; i < THREADCOUNT; i++) SetEvent(goSignal[i]);
			WaitForMultipleObjects(THREADCOUNT, doneSignal, true, INFINITE);

			SDL_UpdateWindowSurface(window->window);
			SDL_FillRect(screen, 0, 0);

			delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene
			if (delta_ticks > 0) {
				fps = CLOCKS_PER_SEC / delta_ticks;
				total += fps;
				counter++;
			}

			printf("Average FPS: %4.0f\n", total / counter);
			//std::cout << "FPS: " << total/counter << std::endl;
		}
#else
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
			HandleEvent(event);

			SDL_FillRect(screen, 0, 0);
			//render here

			for (auto &mesh : meshes)
			{
				mesh.Draw(*rasterizer, color1, color2, ftime);
			}

			SDL_UpdateWindowSurface(window->window);

		delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene
		if (delta_ticks > 0)
			fps = CLOCKS_PER_SEC / delta_ticks;
		std::cout << "FPS: " << fps << std::endl;
	}
#endif
	// Close and destroy the window
	SDL_DestroyWindow(window->window);

	// Clean up
	SDL_Quit();

	return 0;
}