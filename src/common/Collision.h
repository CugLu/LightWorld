#ifndef __COLLISION_H__
#define __COLLISION_H__
#include "Vec3.h"

bool IntersectTriangle(const Vec3& orig, const Vec3& dir, Vec3& v0, Vec3& v1, Vec3& v2, float* t, 
					   float* u, float* v);

bool HitTest(int mouseX, int mouseY);

#endif



