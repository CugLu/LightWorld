#ifndef __VEC4_H__
#define __VEC4_H__

class Vec4
{
public:
	Vec4();
	Vec4(float x, float y, float z, float w);
	Vec4(const Vec3& v, float w);
	Vec4(const Vec4& other);

	~Vec4();

	void operator=( const Vec4& other );

	void Set(float x, float y, float z, float w);
	inline Vec3 &ToVec3( void );

public:
	float x;
	float y;
	float z;
	float w;
};

inline Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}

inline Vec4::Vec4(const Vec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) 
{
}

inline Vec4::Vec4(const Vec4& other):x(other.x), y(other.y), z(other.z), w(other.w) 
{
};

inline Vec4::Vec4()
{
}

inline Vec4::~Vec4()
{
}

inline void Vec4::Set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

inline void Vec4::operator=( const Vec4& other )
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;
}

inline Vec3 &Vec4::ToVec3( void )
{
	return *reinterpret_cast<Vec3 *>(this);
}
#endif // !1



