#pragma once
#include "Shader.h"

#define MAX_CIRCLE_ANGLE 512
#define PI 3.141592653589793
#define HALF_MAX_CIRCLE_ANGLE ( MAX_CIRCLE_ANGLE / 2 )
#define QUARTER_MAX_CIRCLE_ANGLE ( MAX_CIRCLE_ANGLE / 4 )
#define MASK_MAX_CIRCLE_ANGLE ( MAX_CIRCLE_ANGLE - 1 )
#define RadToDegrees = 57.2957795131


class BasicShader : public Shader
{
public:
	BasicShader();
	~BasicShader();
	 EruptionMath::vec3 VertexShader(EruptionMath::vec3 vec3);
	 EruptionMath::Color FragmentShader(EruptionMath::Color color);

	void SetTime(float x) { 
		time = x;

		SetupBeforeRendering();
	};
	void SetPosition(EruptionMath::vec3 x) { pos = x; };
	void SetupBeforeRendering();
	float fastcos(float n)
	{
		float f = n * HALF_MAX_CIRCLE_ANGLE / PI;
		int i = (int)f;
		if (i < 0) return fast_cossin_table[((-i) + QUARTER_MAX_CIRCLE_ANGLE) & MASK_MAX_CIRCLE_ANGLE];
		else return fast_cossin_table[(i + QUARTER_MAX_CIRCLE_ANGLE) & MASK_MAX_CIRCLE_ANGLE];
	}

	float fastsin(float n)
	{
		float f = n * HALF_MAX_CIRCLE_ANGLE / PI;
		int i = (int)f;
		if (i < 0) return fast_cossin_table[(-((-i) & MASK_MAX_CIRCLE_ANGLE)) + MAX_CIRCLE_ANGLE];
		else return fast_cossin_table[i & MASK_MAX_CIRCLE_ANGLE];
	}

	void SetupCosSineTable()
	{
		for (int x = 0; x < MAX_CIRCLE_ANGLE; x++)
		{
			fast_cossin_table[x] = (float)sinf((double)x * PI / HALF_MAX_CIRCLE_ANGLE); // Build cosine look-up table
		}
	}

	float time;
	EruptionMath::vec3 pos;
	EruptionMath::mat4 rot, z, x, projectionMatirx, matTans;
	float fast_cossin_table[MAX_CIRCLE_ANGLE]; // Declare table of fast cosinus and sinus

private:
	float sinz, cosz;
	float sinx, cosx;
};

