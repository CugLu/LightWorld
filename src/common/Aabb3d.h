#ifndef __AABB3D_H__
#define __AABB3D_H__

#include "Vec3.h"

class Aabb3d
{
public:
	Aabb3d();
	~Aabb3d();

	bool AddPoint(const Vec3& v);
	bool RayIntersection( const Vec3 &start, const Vec3 &dir, float &scale ) const;
public:
	Vec3 _min;
	Vec3 _max;
};

#endif



