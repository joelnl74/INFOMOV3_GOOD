#pragma once
#include <vector>

#include "ObjParser.h"
#include "BasicShader.h"
#include "Rasterizer.h"
#include "CLApplication.h"

class Mesh
{
public:
	Mesh(EruptionMath::vec3 position, std::string filename);
	~Mesh();
	void Draw(Rasterizer &rasterizer, EruptionMath::Color color, EruptionMath::Color lineColor, float time, CLApplication &clApplication);
	void SetShader(float time);
	void SortTriangles();
	void DrawTriangles(Rasterizer& raterizer, EruptionMath::Color color, EruptionMath::Color lineColor, CLApplication& clApplication);
	std::vector<EruptionMath::Triangle> verticies;
	BasicShader *shader;
	EruptionMath::vec3 position;
private:
	float fTheta;
	std::vector<EruptionMath::Triangle> toDraw;
	
};

