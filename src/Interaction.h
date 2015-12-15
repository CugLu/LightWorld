#ifndef __INTERACTION_H__
#define __INTERACTION_H__

/*
===============================================================================

	Interaction between entityDef surfaces and a lightDef.

	Interactions with no lightTris and no shadowTris are still
	valid, because they show that a given entityDef / lightDef
	do not interact, even though they share one or more areas.

===============================================================================
*/


typedef struct {		
	// if lightTris == LIGHT_TRIS_DEFERRED, then the calculation of the
	// lightTris has been deferred, and must be done if ambientTris is visible
	srfTriangles_t *		lightTris;

	// shadow volume triangle surface
	srfTriangles_t *		shadowTris;

	// so we can check ambientViewCount before adding lightTris, and get
	// at the shared vertex and possibly shadowVertex caches
	srfTriangles_t *		ambientTris;

	const Material *		shader;
} surfaceInteraction_t;


class Interaction {
public:
	void CreateInteraction(srfTriangles_t* geo, Vec3* lightPos);


private:
	surfaceInteraction_t *	surfaces;
	
public:
	
};



#endif


