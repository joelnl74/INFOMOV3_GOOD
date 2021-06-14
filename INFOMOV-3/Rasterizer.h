#pragma once
#include "SDL.h"
#include "EdgeEquation.h"
#include <vector>

enum RasterizerMode
{
	Line,
	Fill,
	Line_And_Fill
};
class Rasterizer
{
public:
	Rasterizer(SDL_Surface *_surface);
	~Rasterizer();
	Uint32 GetPixel(int x, int y);
	void PutPixel(int x, int y, Uint32 pixel);
	void DrawLine(EruptionMath::vec3 vt1, EruptionMath::vec3 vt2, Uint32 color);
	void DrawTriangle(EruptionMath::Triangle, unsigned int color);
	RasterizerMode mode;
private:
	SDL_Surface *surface;
	std::vector<EruptionMath::vec3> yCoords;
};

