#ifndef __MATH_PLANE_H__
#define __MATH_PLANE_H__

/*
 * 3D plane with equation: a * x + b * y + c * z + d = 0
 */

#include "Vec3.h"

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
	Plane( const Vec3 &normal, const float dist );
	Plane( Vec3 v0, Vec3 v1, Vec3 v2);

	float Distance( const Vec3 &v ) const;
	Vec3& Normal( void );
	void FitThroughPoint( const Vec3 &p );

	float operator[]( const int index ) const;

	float& operator[]( const int index );
private:
	float a;
	float b;
	float c;
	float d;
};


#endif /* !__MATH_PLANE_H__ */
