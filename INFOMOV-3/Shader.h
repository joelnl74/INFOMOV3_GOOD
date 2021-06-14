#pragma once
#include "Math.h"
class Shader
{
public:
	virtual EruptionMath::vec3 VertexShader(EruptionMath::vec3 vec3) = 0;
	virtual EruptionMath::Color FragmentShader(EruptionMath::Color color) = 0;
};

