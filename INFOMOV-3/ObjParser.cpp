#include "ObjParser.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <strstream>

ObjParser::ObjParser()
{
}


ObjParser::~ObjParser()
{
}
std::vector<EruptionMath::Triangle> ObjParser::LoadObj(std::string filename)
{
	std::vector<EruptionMath::Triangle> triangles;
	std::vector<EruptionMath::vec3> vecs;
	std::ifstream f(filename);
	if (!f.is_open())
	{
		printf("Couldnt open filepath");
	}
	while (!f.eof())
	{
		char line[128];
		f.getline(line, 128);

		std::strstream stream;
		stream << line;

		char junk;
		//Vertex
		if (line[0] == 'v')
		{
			EruptionMath::vec3 v;
			stream >> junk >> v.x >> v.y >> v.z;
			vecs.push_back(v);
		}
		//Normal
		if (line[0] == 'f')
		{
			int f[3];
			stream >> junk >> f[0] >> f[1] >> f[2];
			triangles.push_back({ vecs[f[0] - 1], vecs[f[1] - 1], vecs[f[2] - 1] });
		}
	}
	return triangles;
}
