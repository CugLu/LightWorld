#ifndef __VEC4_H__
#define __VEC4_H__
#include "Vec3.h"

class vec4
{
public:
	vec4();
	~vec4();
	vec4(float x, float y, float z, float w);

	vec4(const Vec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

	vec4(const vec4& other) :x(other.x), y(other.y), z(other.z), w(other.w) {};

	inline void operator=( const vec4& other );

	void Set(float x, float y, float z, float w);

	inline Vec3 &ToVec3( void );

public:
	float x;
	float y;
	float z;
	float w;
};

inline vec4::vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}

inline vec4::vec4()
{
}

inline vec4::~vec4()
{
}

inline void vec4::Set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

inline void vec4::operator=( const vec4& other )
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;
}

inline Vec3 &vec4::ToVec3( void )
{
	return *reinterpret_cast<Vec3 *>(this);
}
#endif // !1



