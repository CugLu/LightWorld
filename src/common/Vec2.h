#ifndef __VEC2_H__
#define __VEC2_H__

class Vec2
{
public:
	~Vec2();

	Vec2(): x(0), y(0) {};
	Vec2(float nx, float ny) : x(nx), y(ny){};
	Vec2(const Vec2& other)	:x(other.x), y(other.y) {};

	// operators

	void Zero( void ) {
		x = y = 0.0f;
	}

	Vec2& operator=(const Vec2& other);
	Vec2 operator+(const Vec2& other);
	Vec2& operator+=(const Vec2& other);
	Vec2 operator-(const Vec2& other) const { 
		return Vec2(x - other.x, y - other.y);	
	}

	Vec2& operator-=(const Vec2& other)	{ 
		x-=other.x; y-=other.y; return *this; 
	}

	Vec2 operator*(const Vec2& other) const {
		return Vec2(x * other.x, y * other.y);
	}

	Vec2& operator*=(const Vec2& other)	{ 
		x*=other.x; y*=other.y; return *this; 
	}

	Vec2 operator*(const float v) const { 
		return Vec2(x * v, y * v);	
	}

	Vec2& operator*=(const float v) { 
		x*=v; y*=v; return *this; 
	}

	Vec2 operator/(const Vec2& other) const {
		return Vec2(x / other.x, y / other.y);	
	}

	Vec2& operator/=(const Vec2& other)	{ 
		x/=other.x; y/=other.y; return *this; 
	}

	Vec2 operator/(const float v) const { 
		float i=(float)1.0/v; return Vec2(x * i, y * i);	
	}

	Vec2& operator/=(const float v) { 
		float i=(float)1.0/v; x*=i; y*=i; return *this; 
	}

	bool operator<=(const Vec2&other) const { return x<=other.x && y<=other.y;};
	bool operator>=(const Vec2&other) const { return x>=other.x && y>=other.y;};

	bool operator==(const Vec2& other) const { return other.x==x && other.y==y; }
	bool operator!=(const Vec2& other) const { return other.x!=x || other.y!=y; }

	// functions
	inline float& operator[]( const int index );

	float x, y;

};

inline float& Vec2::operator[]( const int index )
{
	return ( &x )[ index ];
}
#endif /* !__VEC2_H__! */