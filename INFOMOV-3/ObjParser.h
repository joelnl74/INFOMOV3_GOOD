#pragma once
#include <vector> 
#include <string>

#include "Math.h"

class ObjParser
{
public:
	ObjParser();
	~ObjParser();
	std::vector<EruptionMath::Triangle> LoadObj(std::string filename);

};

