#include "Aabb3d.h"

Aabb3d::Aabb3d()
{
}

Aabb3d::~Aabb3d()
{
}

bool Aabb3d::AddPoint(const Vec3& v)
{
	bool expanded = false;
	if ( v.x < _min.x) {
		_min.x = v.x;
		expanded = true;
	}
	if ( v.y < _min.y) {
		_min.y = v.y;
		expanded = true;
	}
	if ( v.z < _min.z ) {
		_min.z = v.z;
		expanded = true;
	}
	if ( v.x > _max.x) {
		_max.x = v.x;
		expanded = true;
	}
	if ( v.y > _max.y ) {
		_max.y = v.y;
		expanded = true;
	}
	if ( v.z > _max.z) {
		_max.z = v.z;
		expanded = true;
	}
	return expanded;
}

bool Aabb3d::RayIntersection( const Vec3 &start, const Vec3 &dir, float &scale ) const 
{
	int i, ax0, ax1, ax2, inside;
	float f;
	Vec3 hit;

	ax0 = -1;
	inside = 0;
	for ( i = 0; i < 3; i++ ) {
		if ( start[i] < _min[i] ) {
			f =  _min[i] - start[i];
		}
		else if ( start[i] > _max[i] ) {
			f = start[i] - _max[i];
		}
		else {
			inside++;
			continue;
		}
		if ( dir[i] == 0.0f ) {
			continue;
		}
		if ( ax0 < 0 ||  f > fabs( scale * dir[i] ) ) {
			scale = - ( f / dir[i] );
			ax0 = i;
		}
	}

	if ( ax0 < 0 ) {
		scale = 0.0f;
		// return true if the start point is inside the bounds
		return ( inside == 3 );
	}

	ax1 = (ax0+1)%3;
	ax2 = (ax0+2)%3;
	hit[ax1] = start[ax1] + scale * dir[ax1];
	hit[ax2] = start[ax2] + scale * dir[ax2];

	return ( hit[ax1] >= _min[ax1] && hit[ax1] <= _max[ax1] &&
		hit[ax2] >= _min[ax2] && hit[ax2] <= _max[ax2] );
}