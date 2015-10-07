#ifndef __MATH_PLANE_H__
#define __MATH_PLANE_H__

/*
===============================================================================

	3D plane with equation: a * x + b * y + c * z + d = 0

===============================================================================
*/

#include "vec3.h"

#define ID_INLINE inline


#define	ON_EPSILON					0.1f
#define DEGENERATE_DIST_EPSILON		1e-4f

#define	SIDE_FRONT					0
#define	SIDE_BACK					1
#define	SIDE_ON						2
#define	SIDE_CROSS					3

// plane sides
#define PLANESIDE_FRONT				0
#define PLANESIDE_BACK				1
#define PLANESIDE_ON				2
#define PLANESIDE_CROSS				3

// plane types
#define PLANETYPE_X					0
#define PLANETYPE_Y					1
#define PLANETYPE_Z					2
#define PLANETYPE_NEGX				3
#define PLANETYPE_NEGY				4
#define PLANETYPE_NEGZ				5
#define PLANETYPE_TRUEAXIAL			6	// all types < 6 are true axial planes
#define PLANETYPE_ZEROX				6
#define PLANETYPE_ZEROY				7
#define PLANETYPE_ZEROZ				8
#define PLANETYPE_NONAXIAL			9

class Plane {
public:
	Plane( void );
	Plane( float a, float b, float c, float d );
	Plane( const vec3 &normal, const float dist );

	float Distance( const vec3 &v ) const;
	int	Side( const vec3 &v, const float epsilon = 0.0f ) const;

private:
	float a;
	float b;
	float c;
	float d;
};

ID_INLINE Plane::Plane( void ) {
}

ID_INLINE Plane::Plane( float a, float b, float c, float d ) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
}

ID_INLINE Plane::Plane( const vec3 &normal, const float dist ) {
	this->a = normal.x;
	this->b = normal.y;
	this->c = normal.z;
	this->d = -dist;
}

ID_INLINE float Plane::Distance( const vec3 &v ) const {
	return a * v.x + b * v.y + c * v.z + d;
}
#endif /* !__MATH_PLANE_H__ */
