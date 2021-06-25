#include "Mesh.h"
#include <algorithm>

Mesh::Mesh(EruptionMath::vec3 vposition, std::string filename)
{
	position = vposition;
	shader = new BasicShader();
	ObjParser parser;
	verticies = parser.LoadObj(filename);

	toDraw.reserve(verticies.size());

	// CHANGED PRE ALLOCATE
	for (int i = 0; i < verticies.size(); i++)
	{
		EruptionMath::Triangle tri;
		toDraw.push_back(tri);
	}
	// ENDCHANGE
}
Mesh::~Mesh()
{
}
void Mesh::Draw(Rasterizer& rasterizer, EruptionMath::Color color, EruptionMath::Color lineColor, float time, CLApplication& clApplication)
{
	fTheta += 1.0f * time;

	unsigned int faceColor = color.toRGB();
	unsigned int linesColor = lineColor.toRGB();

	// CHANGED CAN BE SET ONCE
	if (shader != 0)
	{
		shader->SetTime(fTheta);
		shader->SetPosition(position);
		shader->SetupBeforeRendering();
	}
	// END CHANGE

	// CHANGED TO REFERNECE SO WE DONT MAKE A COPY
	for (int i = 0; i < verticies.size(); i++)
	{
		auto& tri = verticies[i];

		EruptionMath::Triangle& triangle = toDraw[i];

		EruptionMath::vec3 p0 = shader->VertexShader(tri.p[0]);
		EruptionMath::vec3 p1 = shader->VertexShader(tri.p[1]);
		EruptionMath::vec3 p2 = shader->VertexShader(tri.p[2]);

		triangle.p[0] = p0;
		triangle.p[1] = p1;
		triangle.p[2] = p2;
	}
	// ENDCHANGE

	// Changed move this code from inside the loop to outside the loop, more cache friendly
	// Painters algorithm what should be drawn last, so you see those pixels
	std::sort(toDraw.begin(), toDraw.end(), [](EruptionMath::Triangle& tri1, EruptionMath::Triangle& tri2)
		{
			// CHANGE Removed / from code
			float z1 = (tri1.p[0].z + tri1.p[1].z + tri1.p[2].z);
			float z2 = (tri2.p[0].z + tri2.p[1].z + tri2.p[2].z);
			// ENDCHANGE

			return z1 > z2;
		});


	// CHANGED TO REFERENCE.
	for (auto& ver : toDraw)
	{
		// CHANGED TO REFERENCE.
		rasterizer.DrawTriangle(ver, faceColor, linesColor);
	}
	// Changed move this code from inside the loop to outside the loop, more cache friendly
}

void Mesh::SetShader(float time)
{
	fTheta += 1.0f * time;

	// CHANGED CAN BE SET ONCE
	if (shader != 0)
	{
		shader->SetTime(fTheta);
		shader->SetPosition(position);
		shader->SetupBeforeRendering();
	}
	// END CHANGE

	// CHANGED TO REFERNECE SO WE DONT MAKE A COPY
	for (int i = 0; i < verticies.size(); i++)
	{
		auto& tri = verticies[i];

		EruptionMath::Triangle& triangle = toDraw[i];

		EruptionMath::vec3 p0 = shader->VertexShader(tri.p[0]);
		EruptionMath::vec3 p1 = shader->VertexShader(tri.p[1]);
		EruptionMath::vec3 p2 = shader->VertexShader(tri.p[2]);

		triangle.p[0] = p0;
		triangle.p[1] = p1;
		triangle.p[2] = p2;
	}
	// ENDCHANGE
}

void Mesh::SortTriangles()
{
	// Changed move this code from inside the loop to outside the loop, more cache friendly
	// Painters algorithm what should be drawn last, so you see those pixels
	std::sort(toDraw.begin(), toDraw.end(), [](EruptionMath::Triangle& tri1, EruptionMath::Triangle& tri2)
		{
			float z1 = (tri1.p[0].z + tri1.p[1].z + tri1.p[2].z);
			float z2 = (tri2.p[0].z + tri2.p[1].z + tri2.p[2].z);

			return z1 > z2;
		});
}

void Mesh::DrawTriangles(Rasterizer& rasterizer, EruptionMath::Color color, EruptionMath::Color lineColor, CLApplication& clApplication)
{
	unsigned int faceColor = color.toRGB();
	unsigned int linesColor = lineColor.toRGB();

	for (auto& ver : toDraw)
		rasterizer.DrawTriangle(ver, faceColor, linesColor);
}




