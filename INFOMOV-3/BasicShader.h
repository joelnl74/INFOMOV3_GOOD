#pragma once
#include "Shader.h"
class BasicShader : public Shader
{
public:
	BasicShader();
	~BasicShader();
	 EruptionMath::vec3 VertexShader(EruptionMath::vec3 vec3);
	 EruptionMath::Color FragmentShader(EruptionMath::Color color);

	void SetTime(float x) { time = x; };
	void SetPosition(EruptionMath::vec3 x) { pos = x; };

	float time;
	EruptionMath::vec3 pos;
	EruptionMath::mat4 rot, z, x, projectionMatirx;
};

