#include "Collision.h"
#include "Vec3.h"
#include "vec4.h"
#include "Mat4.h"

// Determine whether a ray intersect with a triangle
// Parameters
// orig: origin of the ray
// dir: direction of the ray
// v0, v1, v2: vertices of triangle
// t(out): weight of the intersection for the ray
// u(out), v(out): barycentric coordinate of intersection

bool IntersectTriangle(const Vec3& orig, const Vec3& dir,
					   Vec3& v0, Vec3& v1, Vec3& v2,
					   float* t, float* u, float* v)
{
	// E1
	Vec3 E1 = v1 - v0;

	// E2
	Vec3 E2 = v2 - v0;

	// P
	Vec3 P = dir.cross(E2);

	// determinant
	float det = E1.dot(P);

	// keep det > 0, modify T accordingly
	Vec3 T;
	if( det >0 )
	{
		T = orig - v0;
	}
	else
	{
		T = v0 - orig;
		det = -det;
	}

	// If determinant is near zero, ray lies in plane of triangle
	if( det < 0.0001f )
		return false;

	// Calculate u and make sure u <= 1
	*u = T.dot(P);
	if( *u < 0.0f || *u > det )
		return false;

	// Q
	Vec3 Q = T.cross(E1);

	// Calculate v and make sure u + v <= 1
	*v = dir.dot(Q);
	if( *v < 0.0f || *u + *v > det )
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	*t = E2.dot(Q);

	float fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	return true;
}

Vec3 HitTest(int mouseX, int mouseY, int screenWidth, int screenHeight,
			 mat4 invViewProj, Vec3 viewPos)
{
	float x = (2.0f * mouseX) / screenWidth - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / screenHeight;
	float z = 1.0f;
	Vec3 ray_nds = Vec3 (x, y, z);

	Vec4 ray_clip = Vec4 (ray_nds, 1.0);

	Vec4 ray_world = invViewProj * ray_clip;

	if (ray_world.w != 0.0)
	{
		ray_world.x /= ray_world.w;
		ray_world.y /= ray_world.w;
		ray_world.z /= ray_world.w;			
	}

	Vec3 ray_dir = ray_world.ToVec3() - viewPos;

	return ray_dir;
}