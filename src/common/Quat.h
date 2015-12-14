#ifndef __QUAT_H__
#define __QUAT_H__

#include "Vec3.h"
#include "Mat4.h"
#define QUAT_PI 3.1415926f

class Quat
{
public:
	Quat() :w(1), x(0), y(0), z(0) {};
	Quat(float w, float x, float y, float z);
	~Quat() {};

	Quat operator+(const Quat& b) const;

	Quat operator* (float fScalar) const;
	
	Quat& operator*=(float s);
	
	bool operator==(const Quat& other) const;
	
	bool operator!=(const Quat& other) const;

	Quat& Set(float x, float y, float z, float w);
	
	Quat& MakeIdentity();

	Quat& Normalize();
	
	float Dot(const Quat& q) const;
	
	mat4 ToMatrix() const;
	
	void FromAxisAngle(const Vec3& axis, float angle);
	
	Quat& Slerp(Quat q1, Quat q2, float time, float threshold = 1.0);
	
	Quat& Lerp(Quat q1, Quat q2, float time);
	
	Quat& RotationFromTo(const Vec3& from, const Vec3& to);

	float w, x, y, z;
};

inline Quat Quat::operator+(const Quat& b) const
{
	return Quat(w+b.w, x+b.x, y+b.y, z+b.z);
}

inline Quat Quat::operator* (float fScalar) const
{
	return Quat(fScalar*w,fScalar*x,fScalar*y,fScalar*z);
}

inline Quat& Quat::operator*=(float s)
{
	x*=s;
	y*=s;
	z*=s;
	w*=s;
	return *this;
}

#endif