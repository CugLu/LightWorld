#ifndef __DRAWVERT_H__
#define __DRAWVERT_H__

#include "common/Vec2.h"
#include "common/Vec3.h"

class DrawVert {
public:
	DrawVert() {
		Clear();
	}

	Vec3	xyz;
	Vec2	st;
	Vec3	normal;
	Vec3	tangents[2];
	float   color[4];
 
	void Clear( void );
};

inline void DrawVert::Clear()
{
	xyz.Zero();
	st.Zero();
	normal.Zero();
	tangents[0].Zero();
	tangents[1].Zero();
	color[0] = color[1] = color[2] = 0;
	color[3] = 1.0;
}

#endif



