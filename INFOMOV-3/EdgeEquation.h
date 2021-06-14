#pragma once
#include "Math.h"
struct EdgeEquation {
	float a;
	float b;
	float c;
	bool tie;

	EdgeEquation(const EruptionMath::vec3 v0, const EruptionMath::vec3 &v1)
	{
		a = v0.y - v1.y;
		b = v1.x - v0.x;
		c = -(a * (v0.x + v1.x) + b * (v0.y + v1.y)) / 2;
		tie = a != 0 ? a > 0 : b > 0;
	}

	/// Evaluate the edge equation for the given point. 
	float evaluate(float x, float y)
	{
		return a * x + b * y + c;
	}

	/// Test if the given point is inside the edge. 
	bool test(float x, float y)
	{
		return test(evaluate(x, y));
	}

	/// Test for a given evaluated value. 
	bool test(float v)
	{
		return (v > 0 || v == 0 && tie);
	}
};