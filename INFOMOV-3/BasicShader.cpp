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
	////creating normals
	//EruptionMath::vec3 normal, line1, line2;
	//line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
	//line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
	//line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

	//line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
	//line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
	//line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

	//normal.x = line1.y * line2.z - line1.z * line2.y;
	//normal.y = line1.z * line2.x - line1.x * line2.z;
	//normal.z = line1.x * line2.y - line1.y * line2.x;

	//// It's normally normal to normalise the normal
	//float l = sqrtf(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
	//normal.x /= l; normal.y /= l; normal.z /= l;

	//light_direction.normalize();

	//float dot = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;
	//color.R = 255;

	//triTranslated.color = color;
	return color;
}

