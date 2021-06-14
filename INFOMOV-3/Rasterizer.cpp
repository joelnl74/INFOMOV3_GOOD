#include "Rasterizer.h"
#include <algorithm>
#include <thread>
Rasterizer::Rasterizer(SDL_Surface *_surface)
{
	surface = _surface;
}


Rasterizer::~Rasterizer()
{
}

Uint32 Rasterizer::GetPixel(int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16 *)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32 *)p;
		break;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}
void Rasterizer::PutPixel(int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	if (x < 800 && x >= 0 && y < 600 && y >= 0)
	{
		switch (bpp) {
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p = pixel;
			break;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			}
			else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32 *)p = pixel;
			break;
		}
	}
}


void Rasterizer::DrawLine(EruptionMath::vec3 vt1, EruptionMath::vec3 vt2, Uint32 color)
{
	float xdiff = (vt2.x - vt1.x);
	float ydiff = (vt2.y - vt1.y);

	if (xdiff == 0.0f && ydiff == 0.0f) {
		PutPixel(vt1.x, vt1.y, color);
		return;
	}

	if (fabs(xdiff) > fabs(ydiff)) {
		float xmin, xmax;

		// set xmin to the lower x value given
		// and xmax to the higher value
		if (vt1.x < vt2.x) {
			xmin = vt1.x;
			xmax = vt2.x;
		}
		else {
			xmin = vt2.x;
			xmax = vt1.x;
		}

		// draw line in terms of y slope
		float slope = ydiff / xdiff;
		for (float x = xmin; x <= xmax; x += 1.0f) {
			float y = vt1.y + ((x - vt1.x) * slope);
			PutPixel(x, y, color);
		}
	}
	else {
		float ymin, ymax;

		// set ymin to the lower y value given
		// and ymax to the higher value
		if (vt1.y < vt2.y) {
			ymin = vt1.y;
			ymax = vt2.y;
		}
		else {
			ymin = vt2.y;
			ymax = vt1.y;
		}

		// draw line in terms of x slope
		float slope = xdiff / ydiff;
		for (float y = ymin; y <= ymax; y += 1.0f) {
			float x = vt1.x + ((y - vt1.y) * slope);
			PutPixel(x, y, color);
		}
	}
}
void Rasterizer::DrawTriangle(EruptionMath::Triangle tri, unsigned int color)
{
	EruptionMath::Color color2(0, 0, 125);

	if (mode == RasterizerMode::Line || mode == RasterizerMode::Line_And_Fill)
	{
		//std::thread thread1(std::bind(&Rasterizer::DrawLine, this, std::ref(tri.p[0]), std::ref(tri.p[1]), color2.toRGB()));
		//std::thread thread2(std::bind(&Rasterizer::DrawLine, this, std::ref(tri.p[1]), std::ref(tri.p[2]), color2.toRGB()));
		//std::thread thread3(std::bind(&Rasterizer::DrawLine, this, std::ref(tri.p[2]), std::ref(tri.p[1]), color2.toRGB()));
		
		DrawLine(tri.p[0], tri.p[1], color2.toRGB());
		DrawLine(tri.p[1], tri.p[2], color2.toRGB());
		DrawLine(tri.p[2], tri.p[0], color2.toRGB());
	}

	if (mode == RasterizerMode::Fill || mode == RasterizerMode::Line_And_Fill)
	{
		//Half space triangle
		// Compute triangle bounding box. 
		int minX = std::min(std::min(tri.p[0].x, tri.p[1].x), tri.p[2].x);
		int maxX = std::max(std::max(tri.p[0].x, tri.p[1].x), tri.p[2].x);
		int minY = std::min(std::min(tri.p[0].y, tri.p[1].y), tri.p[2].y);
		int maxY = std::max(std::max(tri.p[0].y, tri.p[1].y), tri.p[2].y);

		// Compute edge equations. 
		EdgeEquation e0(tri.p[0], tri.p[1]);
		EdgeEquation e1(tri.p[1], tri.p[2]);
		EdgeEquation e2(tri.p[2], tri.p[0]);

		float area = 0.5 * (e0.c + e1.c + e2.c);

		// Add 0.5 to sample at pixel centers. 
		for (float x = minX + 0.5f, xm = maxX + 0.5f; x <= xm; x += 1.0f)
			for (float y = minY + 0.5f, ym = maxY + 0.5f; y <= ym; y += 1.0f)
			{
				if (e0.test(x, y) && e1.test(x, y) && e2.test(x, y))
				{
					PutPixel(x, y, color);
				}
			}
	}
}
