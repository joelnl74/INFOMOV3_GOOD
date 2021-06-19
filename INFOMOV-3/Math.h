#pragma once
#include <cmath>
#include "immintrin.h" // for AVX 
#include "nmmintrin.h" // for SSE4.2

namespace EruptionMath
{
	struct vec3
	{
		float x;
		float y;
		float z;
		float w;

		vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) : x(x), y(y), z(z), w(w) {}
		//add
		vec3 operator +(const vec3 &rhs)
		{
			return vec3(x + rhs.x, y + rhs.y, z + rhs.z);
		};
		//subtract
		vec3 operator -(const vec3 &rhs)
		{
			return vec3(x - rhs.x, y - rhs.y, z - rhs.z);
		}
		//?
		vec3 operator / (float k)
		{
			return vec3(x / k,y /k, z / k);
		}
		//get Magnitude of the vector
		__forceinline float GetMagnitude()
		{
			return sqrtf(x * x + y * y + z * z);
		}
		//multiply by a scaler value
		vec3 operator*(float num) const
		{
			return vec3(x * num, y * num, z * num);
		}
		//normalize vector
		__forceinline vec3 normalize()
		{
			float magnitude = sqrtf(x * x + y * y + z * z);
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;

			return vec3(x, y, z);
		}
		//calculate and return dot product
		__forceinline float DotProduct(const vec3 &vec, const vec3 &vec1) const
		{
			return (vec.x * vec1.x) + (vec.y * vec1.y) + (vec.z * vec1.z);
		}
		//calculate and return cross product
		__forceinline vec3 CrossProduct(const vec3 &vec) const
		{
			return vec3(y * vec.z - z * vec.y,
				z * vec.x - x * vec.z,
				x * vec.y - y * vec.x);
		}
	};
	struct vec2
	{
		float x;
		float y;

		vec2(float x, float y) : x(x), y(y) {}

		//add
		vec2 operator +(const vec3 &rhs)
		{
			return vec2(x + rhs.x, y + rhs.y);
		};
		//subtract
		vec2 operator -(const vec3 &rhs)
		{
			return vec2(x - rhs.x, y - rhs.y);
		}
		//multiply by a scaler value
		vec2 operator*(float num) const
		{
			return vec2(x * num, y * num);
		}
		//normalize
		__forceinline void normalize()
		{
			float magnitude = sqrtf(x * x + y * y);
			x /= magnitude;
			y /= magnitude;
		}
	};

	struct Color
	{
		float R, G, B;

		Color(float r = 255, float g = 255, float b = 255) { R = r, G = g, B = b; };

		Color operator + (const Color &c) const
		{
			return (R + c.R, G + c.G, B + c.B);
		}
		Color operator - (const Color &c) const
		{
			return (R - c.R, G - c.G, B - c.B);
		}
		Color operator * (float f) const
		{
			return (R * f, G * f, B * f);
		}
		int toRGB()
		{
			unsigned int r = R;
			unsigned int g = G;
			unsigned int b = B;

			return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);;  // 16742927
		}
	};
	struct  Triangle
	{
		vec3 p[3];
		Color color;
		Triangle(vec3 vt1, vec3 vt2, vec3 vt3)
		{
			p[0] = vt1;
			p[1] = vt2;
			p[2] = vt3;
		}

	};
	struct mat4
	{
		float m4[4][4] = { 0 };

		__forceinline mat4 Matrix_MultiplyMatrix(mat4 &m1, mat4 &m2)
		{
			// Changed reorder loop Increase FPS by one.
			mat4 matrix{};
			for (int r = 0; r < 4; r++)
				for (int c = 0; c < 4; c++)
					matrix.m4[r][c] = m1.m4[r][0] * m2.m4[0][c] + m1.m4[r][1] * m2.m4[1][c] + m1.m4[r][2] * m2.m4[2][c] + m1.m4[r][3] * m2.m4[3][c];
			// Changed reorder loop Increase FPS by one.

			return matrix;
		}

		//Create a projection matrix
		__forceinline mat4 ProjectionMatirx(float fNearr, float fFarr, float fFovv, float width, float height)
		{
			mat4 projection;

			float fNear = fNearr;
			float fFar = fFarr;
			float fFov = fFovv;
			float fAspectRatio = (float)width / (float)height;
			float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

			projection.m4[0][0] = fAspectRatio * fFovRad;
			projection.m4[1][1] = fFovRad;
			projection.m4[2][2] = fFar / (fFar - fNear);
			projection.m4[3][2] = (-fFar * fNear) / (fFar - fNear);
			projection.m4[2][3] = 1.0f;
			projection.m4[3][3] = 0.0f;

			return projection;
		};
		__forceinline mat4 RotateZ(float sin, float cos)
		{
			mat4 matRotZ;

			// Rotation Z
			matRotZ.m4[0][0] = cos;
			matRotZ.m4[0][1] = sin;
			matRotZ.m4[1][0] = -sin;
			matRotZ.m4[1][1] = cos;
			matRotZ.m4[2][2] = 1.0f;
			matRotZ.m4[3][3] = 1.0f;
			return matRotZ;
		}
		__forceinline mat4 RotateX(float sin, float cos)
		{
			mat4 matRotX;

			// Rotation X
			matRotX.m4[0][0] = 1.0f;
			matRotX.m4[1][1] = cos;
			matRotX.m4[1][2] = sin;
			matRotX.m4[2][1] = -sin;
			matRotX.m4[2][2] = cos;
			matRotX.m4[3][3] = 1.0f;

			return matRotX;
		};
		__forceinline mat4 Identity()
		{
			mat4 matrix;

			matrix.m4[0][0] = 1.0f;
			matrix.m4[1][1] = 1.0f;
			matrix.m4[2][2] = 1.0f;
			matrix.m4[3][3] = 1.0f;

			return matrix;
		}
		__forceinline mat4 Matirx_Translation(float x, float y, float z)
		{
			mat4 matrix;
			matrix.m4[0][0] = 1.0f;
			matrix.m4[1][1] = 1.0f;
			matrix.m4[2][2] = 1.0f;
			matrix.m4[3][3] = 1.0f;
			matrix.m4[3][0] = x;
			matrix.m4[3][1] = y;
			matrix.m4[3][2] = z;
			return matrix;
		}
		//Multiply a vector by a matrix
		__forceinline vec3 MulitiplyMatrixVector(vec3 &v, mat4 &m)
		{
			// BEGIN REFERENCE CODE
			// vec3 v;
			// v.x = i.x * m.m4[0][0] + i.y * m.m4[1][0] + i.z * m.m4[2][0] + i.w * m.m4[3][0];
			// v.y = i.x * m.m4[0][1] + i.y * m.m4[1][1] + i.z * m.m4[2][1] + i.w * m.m4[3][1];
			// v.z = i.x * m.m4[0][2] + i.y * m.m4[1][2] + i.z * m.m4[2][2] + i.w * m.m4[3][2];
			// v.w = i.x * m.m4[0][3] + i.y * m.m4[1][3] + i.z * m.m4[2][3] + i.w * m.m4[3][3];
			// END REFERENCE CODE

			// PUT MATRIX DATA IN SIMD VAR.
			__m128 i0 = { m.m4[0][0], m.m4[1][0], m.m4[2][0], m.m4[3][0] };
			__m128 i1 = { m.m4[0][1], m.m4[1][1], m.m4[2][1], m.m4[3][1] };
			__m128 i2 = { m.m4[0][2], m.m4[1][2], m.m4[2][2], m.m4[3][2] };
			__m128 i3 = { m.m4[0][3], m.m4[1][3], m.m4[2][3], m.m4[3][3] };

			// PUT THE VECTOR IN SIMD VAR.
			__m128 v4 = { v.x, v.y, v.z, v.w };

			// Multiply packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst.
			__m128 m0 = _mm_mul_ps(v4, i0);
			__m128 m1 = _mm_mul_ps(v4, i1);
			__m128 m2 = _mm_mul_ps(v4, i2);
			__m128 m3 = _mm_mul_ps(v4, i3);

			// Unpack and interleave single-precision (32-bit) floating-point elements from the low half of a and b, and store the results in dst.
			__m128 u0 = _mm_unpacklo_ps(m0, m1);
			// Unpack and interleave single-precision (32-bit) floating-point elements from the higher half of a and b, and store the results in dst.
			__m128 u1 = _mm_unpackhi_ps(m0, m1);
			// Add them together
			__m128 a0 = _mm_add_ps(u0, u1);

			// Unpack and interleave single-precision (32-bit) floating-point elements from the low half of a and b, and store the results in dst.
			__m128 u2 = _mm_unpacklo_ps(m2, m3);
			// Unpack and interleave single-precision (32-bit) floating-point elements from the higher half of a and b, and store the results in dst.
			__m128 u3 = _mm_unpackhi_ps(m2, m3);
			// Add them together
			__m128 a1 = _mm_add_ps(u2, u3);

			// Unpack and interleave single-precision (32-bit) floating-point elements from the low half of a and b, and store the results in dst.
			__m128 f0 = _mm_movelh_ps(a0, a1);
			// Unpack and interleave single-precision (32-bit) floating-point elements from the higher half of a and b, and store the results in dst.
			__m128 f1 = _mm_movehl_ps(a1, a0);
			// Add them together
			__m128 f2 = _mm_add_ps(f0, f1);

			return vec3{ f2.m128_f32[0], f2.m128_f32[1], f2.m128_f32[2], f2.m128_f32[3] };
		}
	};
};



