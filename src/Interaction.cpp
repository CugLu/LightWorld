#include "Interaction.h"
#include "common/Plane.h"
#include "r_public.h"

#define	MAX_SHADOW_INDEXES		0x18000
#define	MAX_SHADOW_VERTS		0x18000
#define MAX_FACE_PLANE 0x1000

static int	numShadowIndices;
static unsigned short shadowIndices[MAX_SHADOW_INDEXES];

static int numFacePlane;
static int facePlaneIndices[MAX_FACE_PLANE];

srfTriangles_t* R_GenerateCaps( Vec3 lightPos, srfTriangles_t* tri )
{
	if (!tri->facePlanes || !tri->facePlanesCalculated)
		R_DeriveFacePlanes(tri);

	int numPlanes = tri->numIndexes / 3; 
	Plane* plane = tri->facePlanes;
	for (int i=0; i<numPlanes; i++)
	{
		if(plane[i].Distance(lightPos) > 0)
			facePlaneIndices[numPlanes++] = i;
	}

	srfTriangles_t* tris = R_AllocStaticTriSurf();
	R_AllocStaticTriSurfVerts(tri, numPlanes * 3);

	return NULL;
}

void Interaction::CreateInteraction( srfTriangles_t* geo, Vec3* lightPos )
{

}
