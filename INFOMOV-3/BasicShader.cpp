#include "BasicShader.h"
#include "Math.h"

BasicShader::BasicShader()
{
	//Create a projection matrix
	projectionMatirx =  projectionMatirx.ProjectionMatirx(0.1f, 1000.0f, 90.0f, 800, 600);;
}


BasicShader::~BasicShader()
{
}
EruptionMath::vec3 BasicShader::VertexShader(EruptionMath::vec3 vec3)
{
	EruptionMath::vec3 transformed, projected;
	EruptionMath::mat4 matTans, matWorld;

	matTans = matTans.Matirx_Translation(0, 0, 12.0f);
	z = z.RotateZ(time * 0.5f); 
	x = x.RotateX(time);

	//Matrix multiplication
	matWorld = matWorld.Identity();
    matWorld = matWorld.Matrix_MultiplyMatrix(z, x);
	matWorld = matWorld.Matrix_MultiplyMatrix(matWorld, matTans);

	transformed = matWorld.MulitiplyMatrixVector(vec3, matWorld);

	projected = matWorld.MulitiplyMatrixVector(transformed, projectionMatirx);
	
	projected = projected / projected.w;
	//set into screen space
	projected.x += 1; projected.y += 1;
	//change position of the object
	projected.x *= pos.x;
	projected.y *= pos.y;

	return projected;
}
EruptionMath::Color BasicShader::FragmentShader(EruptionMath::Color color)
{
	return color;
}

