#include "Quat.h"

Quat::Quat(float _w, float _x, float _y, float _z) :w(_w), x(_x), y(_y), z(_z)
{

}

void Quat::fromAxisAngle(const Vec3& axis, float angle)
{
	float rad = angle*0.5f;
	float scale = sinf(rad);

	w = cos(rad);
	x = axis.x * scale;
	y = axis.y * scale;
	z = axis.z * scale;
}

mat4 Quat::toMatrix() const
{
	mat4 m;
	float wx, wy, wz, xx, xy, xz, yy, yz, zz;

	// calculate coefficients
	xx = x * x * 2.f;
	xy = x * y * 2.f;
	xz = x * z * 2.f;
	
	yy = y * y * 2.f;
	yz = y * z * 2.f;
	
	zz = z * z * 2.f;

	wx = w * x * 2.f;
	wy = w * y * 2.f;
	wz = w * z * 2.f;

	m.m[0]	= 1.0f - (yy + zz);
	m.m[4]	= xy - wz;
	m.m[8]	= xz + wy;
	m.m[12] = 0;

	m.m[1]  = xy + wz;
	m.m[5]  = 1.0f - (xx + zz);
	m.m[9]  = yz - wx;
	m.m[13] = 0;

	m.m[2] = xz - wy;
	m.m[6] = yz + wx;
	m.m[10] = 1.0f - (xx + yy);
	m.m[14]= 0;

	m.m[3] = 0;
	m.m[7] = 0;
	m.m[11] = 0;
	m.m[15] = 1;
	return m;
}

float Quat::dot(const Quat& rkQ) const
{
    return w*rkQ.w+x*rkQ.x+y*rkQ.y+z*rkQ.z;
}

Quat& Quat::slerp(Quat q1, Quat q2, float time, float threshold)
{
	float angle = q1.dot(q2);

	// make sure we use the short rotation
	if (angle < 0.0f)
	{
		q1 *= -1.0f;
		angle *= -1.0f;
	}

	if (angle <= (1-threshold)) // spherical interpolation
	{
		const float theta = acosf(angle);
		const float invsintheta = 1/(sinf(theta));
		const float scale = sinf(theta * (1.0f-time)) * invsintheta;
		const float invscale = sinf(theta * time) * invsintheta;
		return (*this = (q1*scale) + (q2*invscale));
	}
	else // linear interploation
		return lerp(q1,q2,time);
}

Quat& Quat::lerp(Quat q1, Quat q2, float time)
{
	const float scale = 1.0f - time;
	return (*this = (q1*scale) + (q2*time));
}

Quat& Quat::rotationFromTo(const Vec3& from, const Vec3& to)
{
	// Based on Stan Melax's article in Game Programming Gems
	// Copy, since cannot modify local
	Vec3 v0 = from;
	Vec3 v1 = to;
	v0.normalize();
	v1.normalize();

	const float d = v0.dot(v1);
	if (d >= 1.0f) // If dot == 1, vectors are the same
	{
		return MakeIdentity();
	}
	else if (d <= -1.0f) // exactly opposite
	{
		Vec3 axis(1.0f, 0.f, 0.f);
		axis = axis.cross(v0);
		if (axis.getLength()==0)
		{
			axis.set(0.f,1.f,0.f);
			axis = axis.cross(v0);
		}
		// same as fromAngleAxis(core::PI, axis).normalize();
		return set(axis.x, axis.y, axis.z, 0).Normalize();
	}

	const float s = sqrtf( (1+d)*2 ); // optimize inv_sqrt
	const float invs = 1.f / s;
	const Vec3 c = v0.cross(v1)*invs;
	return set(c.x, c.y, c.z, s * 0.5f).Normalize();
}

Quat& Quat::MakeIdentity()
{
	w = 1.f;
	x = 0.f;
	y = 0.f;
	z = 0.f;
	return *this;
}


Quat& Quat::set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
	return *this;
}

Quat& Quat::Normalize()
{
	const float n = x*x + y*y + z*z + w*w;

	if (n == 1)
		return *this;

	//n = 1.0f / sqrtf(n);
	return (*this *= (1.f / sqrtf(n)) );
}