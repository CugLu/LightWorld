#ifndef __INTERACTION_H__
#define __INTERACTION_H__

#include "r_public.h"
/*
===============================================================================

	Interaction between entityDef surfaces and a lightDef.

	Interactions with no lightTris and no shadowTris are still
	valid, because they show that a given entityDef / lightDef
	do not interact, even though they share one or more areas.

===============================================================================
*/



class Interaction {
public:
	Interaction();
	~Interaction();

	void Interaction::CreateInteraction( srfTriangles_t* tri, Vec3& lightPos, mat4& modelMatrix );


private:
	
public:
	srfTriangles_t *		shadowTris;

	int numSilIndices;
	int numCapIndices;
public:
	
};



#endif


