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

Plane::Plane( Vec3 v0, Vec3 v1, Vec3 v2 )
 {
	Vec3 normal = (v1 - v0).cross(v2 - v0);
	normal.normalize();
	a = normal.x;
	b = normal.y;
	c = normal.z;
	FitThroughPoint(v0);
 }

float Plane::Distance( const Vec3 &v ) const {
	return a * v.x + b * v.y + c * v.z + d;
}

void Plane::FitThroughPoint( const Vec3 &p ) {
	d = -( Normal().dot(p) );
}

Vec3& Plane::Normal( void ) {
	return *reinterpret_cast<Vec3 *>(&a);
}

float Plane::operator[]( const int index ) const
{
	return ( &a )[ index ];
}

float& Plane::operator[]( const int index )
{
	return ( &a )[ index ];
}

