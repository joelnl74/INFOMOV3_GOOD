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

	for (int i = 0; i < 800; i++)
	{
		for (int j = 0; j < 600; j++)
		{
			depthMap[i + 800 * j] = INT_MAX;
		}
	}

}
Mesh::~Mesh()
{
}
void Mesh::Draw(Rasterizer &raterizer, EruptionMath::Color color, EruptionMath::Color lineColor, float time)
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

	for (int i = 0; i < 800; i++)
	{
		for (int j = 0; j < 600; j++)
		{
			depthMap[i + 800 * j] = INT_MAX;
		}
	}

	// CHANGED TO REFERNECE SO WE DONT MAKE A COPY
	for (int i = 0; i < verticies.size(); i++)
	{
		auto& tri = verticies[i];

		EruptionMath::Triangle &triangle = toDraw[i];

		EruptionMath::vec3 p0 = shader->VertexShader(tri.p[0]);
		EruptionMath::vec3 p1 = shader->VertexShader(tri.p[1]);
		EruptionMath::vec3 p2 = shader->VertexShader(tri.p[2]);

		triangle.p[0] = p0;
		triangle.p[1] = p1;
		triangle.p[2] = p2;
	}
	// ENDCHANGE


	// CHANGED TO REFERENCE.
	for (auto &ver : toDraw)
	{
	// CHANGED TO REFERENCE.
		auto& vertex1 = ver.p[0];
		auto& vertex2 = ver.p[1];
		auto& vertex3 = ver.p[2];

		int index1 = vertex1.x + 800 * vertex1.y;
		int index2 = vertex2.x + 800 * vertex2.y;
		int index3 = vertex3.x + 800 * vertex3.y;

		auto& depth1 = depthMap[index1];
		auto& depth2 = depthMap[index2];
		auto& depth3 = depthMap[index3];

		if (depth1 > vertex1.z || depth2 > vertex2.z || depth3 > vertex3.z)
		{
			raterizer.DrawTriangle(ver, faceColor, linesColor);

			depth1 = vertex1.z;
			depth2 = vertex2.z;
			depth3 = vertex3.z;

		}
	}
	// Changed move this code from inside the loop to outside the loop, more cache friendly
}

