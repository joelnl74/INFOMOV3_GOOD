#include "Mesh.h"
#include <algorithm>

Mesh::Mesh(EruptionMath::vec3 vposition, std::string filename)
{
	position = vposition;
	shader = new BasicShader();
	ObjParser parser;
	verticies = parser.LoadObj(filename);
}
Mesh::~Mesh()
{
}
void Mesh::Draw(Rasterizer &raterizer, EruptionMath::Color color, EruptionMath::Color lineColor, float time)
{
	fTheta += 1.0f * time;

	std::vector<EruptionMath::Triangle> toDraw;

	// CHANGED CAN BE SET ONCE
	if (shader != 0)
	{
		shader->SetTime(fTheta);
		shader->SetPosition(position);
		shader->SetupBeforeRendering();
	}
	// END CHANGE

	// CHANGED TO REFERNECE SO WE DONT MAKE A COPY
	for (auto &tri : verticies)
	{
	// ENDCHANGE
		EruptionMath::Triangle triangle;
		
		triangle.p[0] = shader->VertexShader(tri.p[0]);
		triangle.p[1] = shader->VertexShader(tri.p[1]);
		triangle.p[2] = shader->VertexShader(tri.p[2]);

		toDraw.push_back(triangle);
	}

	// Changed move this code from inside the loop to outside the loop, more cache friendly
	// Painters algorithm what should be drawn last, so you see those pixels
	std::sort(toDraw.begin(), toDraw.end(), [](EruptionMath::Triangle& tri1, EruptionMath::Triangle& tri2)
	{
		float z1 = (tri1.p[0].z + tri1.p[1].z + tri1.p[2].z) / 3.0f;
		float z2 = (tri2.p[0].z + tri2.p[1].z + tri2.p[2].z) / 3.0f;
		
		return z1 > z2;
	});

	// CHANGED TO REFERENCE.
	for (auto &ver : toDraw)
	{
	// CHANGED TO REFERENCE.
		raterizer.DrawTriangle(ver, color.toRGB(), lineColor);
	}
	// Changed move this code from inside the loop to outside the loop, more cache friendly
}

