#include "Mat4.h"

void mat4::MakeIdentity()
{
	memset(m, 0, sizeof(float)* 16);   // memory.h
	m[0] = m[5] = m[10] = m[15] = 1.f;
}

void mat4::BuildPerspectiveProjection(float fieldOfViewRadians, float aspectRatio, float zNear, float zFar)
{
	float cotangent = (float)(cos(fieldOfViewRadians/2) / sin(fieldOfViewRadians/2));

	m[0] = cotangent / aspectRatio;
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;

	m[4] = 0;
	m[5] = cotangent;
	m[6] = 0;
	m[7] = 0;
	
	m[8] = 0;
	m[9] = 0;
	m[10] = (zFar+zNear)/(zNear-zFar);
//	M[10] = (zFar/(zNear-zFar)); // DirectX version
	m[11] = -1;

	m[12] = 0;
	m[13] = 0;
	m[14] = 2*zNear*zFar/(zNear-zFar);
//	M[14] = (zNear*zFar/(zNear-zFar)); // DirectX version
	m[15] = 0;
}

void mat4::TransformVec3(float& x, float& y, float& z)
{
	float vector[3];

	vector[0] = x*m[0] + y*m[4] + z*m[8] + m[12];
	vector[1] = x*m[1] + y*m[5] + z*m[9] + m[13];
	vector[2] = x*m[2] + y*m[6] + z*m[10] + m[14];

	x = vector[0];
	y = vector[1];
	z = vector[2];
}

void mat4::SetScale(float x, float y, float z)
{
	m[0] = x;
	m[5] = y;
	m[10] = z;
}

void mat4::MultiplyWith1x4Matrix(float *matrix)
{
	float mat[4];
	mat[0] = matrix[0];
	mat[1] = matrix[1];
	mat[2] = matrix[2];
	mat[3] = matrix[3];

	matrix[0] = m[0]*mat[0] + m[4]*mat[1] + m[8]*mat[2] + m[12]*mat[3];
	matrix[1] = m[1]*mat[0] + m[5]*mat[1] + m[9]*mat[2] + m[13]*mat[3];
	matrix[2] = m[2]*mat[0] + m[6]*mat[1] + m[10]*mat[2] + m[14]*mat[3];
	matrix[3] = m[3]*mat[0] + m[7]*mat[1] + m[11]*mat[2] + m[15]*mat[3];
}

mat4 mat4::Inverse()
{
	mat4 r;
	float v0 = m[2] * m[7] - m[6] * m[3];
    float v1 = m[2] * m[11] - m[10] * m[3];
    float v2 = m[2] * m[15] - m[14] * m[3];
    float v3 = m[6] * m[11] - m[10] * m[7];
    float v4 = m[6] * m[15] - m[14] * m[7];
    float v5 = m[10] * m[15] - m[14] * m[11];

    float t00 = + (v5 * m[5] - v4 * m[9] + v3 * m[13]);
    float t10 = - (v5 * m[1] - v2 * m[9] + v1 * m[13]);
    float t20 = + (v4 * m[1] - v2 * m[5] + v0 * m[13]);
    float t30 = - (v3 * m[1] - v1 * m[5] + v0 * m[9]);

    float invDet = 1 / (t00 * m[0] + t10 * m[4] + t20 * m[8] + t30 * m[12]);

    r.m[0] = t00 * invDet;
    r.m[1] = t10 * invDet;
    r.m[2] = t20 * invDet;
    r.m[3] = t30 * invDet;

    r.m[4] = - (v5 * m[4] - v4 * m[8] + v3 * m[12]) * invDet;
    r.m[5] = + (v5 * m[0] - v2 * m[8] + v1 * m[12]) * invDet;
    r.m[6] = - (v4 * m[0] - v2 * m[4] + v0 * m[12]) * invDet;
    r.m[7] = + (v3 * m[0] - v1 * m[4] + v0 * m[8]) * invDet;

    v0 = m[1] * m[7] - m[5] * m[3];
    v1 = m[1] * m[11] - m[9] * m[3];
    v2 = m[1] * m[15] - m[13] * m[3];
    v3 = m[5] * m[11] - m[9] * m[7];
    v4 = m[5] * m[15] - m[13] * m[7];
    v5 = m[9] * m[15] - m[13] * m[11];

    r.m[8] = + (v5 * m[4] - v4 * m[8] + v3 * m[12]) * invDet;
    r.m[9] = - (v5 * m[0] - v2 * m[8] + v1 * m[12]) * invDet;
    r.m[10] = + (v4 * m[0] - v2 * m[4] + v0 * m[12]) * invDet;
    r.m[11] = - (v3 * m[0] - v1 * m[4] + v0 * m[8]) * invDet;

    v0 = m[6] * m[1] - m[2] * m[5];
    v1 = m[10] * m[1] - m[2] * m[9];
    v2 = m[14] * m[1] - m[2] * m[13];
    v3 = m[10] * m[5] - m[6] * m[9];
    v4 = m[14] * m[5] - m[6] * m[13];
    v5 = m[14] * m[9] - m[10] * m[13];

    r.m[12] = - (v5 * m[4] - v4 * m[8] + v3 * m[12]) * invDet;
    r.m[13] = + (v5 * m[0] - v2 * m[8] + v1 * m[12]) * invDet;
    r.m[14] = - (v4 * m[0] - v2 * m[4] + v0 * m[12]) * invDet;
    r.m[15] = + (v3 * m[0] - v1 * m[4] + v0 * m[8]) * invDet;
	return r;
}

void mat4::BuildProjectionOrthoRH(float widthOfViewVolume, float heightOfViewVolume, float zNear, float zFar)
{
	m[0] = 2/widthOfViewVolume;
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;

	m[4] = 0;
	m[5] = 2/heightOfViewVolume;
	m[6] = 0;
	m[7] = 0;

	m[8] = 0;
	m[9] = 0;
	m[10] = 1/(zNear-zFar);
	m[11] = 0;

	m[12] = 0;
	m[13] = 0;
	m[14] = zNear/(zNear-zFar);
	m[15] = 1;
}