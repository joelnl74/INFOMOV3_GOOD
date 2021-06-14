#pragma once
#include <vector>

#include "ObjParser.h"
#include "BasicShader.h"
#include "Rasterizer.h"
class Mesh
{
public:
	Mesh(EruptionMath::vec3 position, std::string filename);
	~Mesh();
	void Draw(Rasterizer &raterizer, EruptionMath::Color color, float time);
	std::vector<EruptionMath::Triangle> verticies;
	BasicShader *shader;
	EruptionMath::vec3 position;
private:
	float fTheta;

	
};

