#ifndef __PICK_H__
#define __PICK_H__

#include "glutils.h"
#include "r_public.h"

#define MAX_INDICE 1000

class Pick
{
public:
	Pick();
	~Pick();

	void PickUp(srfTriagles_t* srf, Vec3 orig, Vec3 dir);

private:
	int numIndice;
	glIndex_t* indice;
	srfTriangles_t* geo;
};

#endif