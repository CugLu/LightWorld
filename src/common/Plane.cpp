#include "Plane.h"
 Plane::Plane( void ) {
}

 Plane::Plane( float a, float b, float c, float d ) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
}

 Plane::Plane( const Vec3 &normal, const float dist ) {
	this->a = normal.x;
	this->b = normal.y;
	this->c = normal.z;
	this->d = -dist;
}

 float Plane::Distance( const Vec3 &v ) const {
	return a * v.x + b * v.y + c * v.z + d;
}