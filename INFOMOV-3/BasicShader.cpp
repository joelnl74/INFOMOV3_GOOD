#pragma once
#include "BasicShader.h"

BasicShader::BasicShader()
{
	//Create a projection matrix
	projectionMatirx =  projectionMatirx.ProjectionMatirx(0.1f, 1000.0f, 90.0f, 800, 600);
	matTans = matTans.Matirx_Translation(0, 0, 12.0f);

	time = 0;

	SetupCosSineTable();
}


BasicShader::~BasicShader()
{
}
EruptionMath::vec3 BasicShader::VertexShader(EruptionMath::vec3 vec3)
{
	EruptionMath::vec4 vec;
	vec.x = vec3.x;
	vec.y = vec3.y;
	vec.z = vec3.z;
	vec.w = 1.0f;
	EruptionMath::vec4 transformed, projected;
	EruptionMath::mat4 matWorld = rotationMat;

	//Matrix multiplication
	transformed = matWorld.MulitiplyMatrixVector(vec, matWorld);
	projected = matWorld.MulitiplyMatrixVector(transformed, projectionMatirx);

	projected = projected / projected.w;
	//set into screen space
	projected.x += 1; projected.y += 1;

	//change position of the object
	projected.x *= pos.x;
	projected.y *= pos.y;

	EruptionMath::vec3 test;
	test.x = projected.x;
	test.y = projected.y;
	test.z = projected.z;
	return test;
}
EruptionMath::Color BasicShader::FragmentShader(EruptionMath::Color color)
{
	return color;
}

void BasicShader::SetupBeforeRendering()
{
	EruptionMath::mat4 matWorld;

	sinz = fastsin(time * 0.5f);
	cosz = fastcos(time * 0.5f);

	sinx = fastsin(time);
	cosx = fastcos(time);

	z = z.RotateZ(sinz, cosz);
	x = x.RotateX(sinx, cosx);

	rotationMat = matWorld.Matrix_MultiplyMatrix(z, x);
	rotationMat = matWorld.Matrix_MultiplyMatrix(rotationMat, matTans);
}

