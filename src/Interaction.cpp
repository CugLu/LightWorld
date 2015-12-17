#include "Interaction.h"
#include "common/Plane.h"
#include "r_public.h"
#include "sys/sys_public.h"

#define	MAX_SHADOW_INDEXES		0x18000
#define	MAX_SHADOW_VERTS		0x18000
#define MAX_FACE_PLANE 0x1000
#define MAX_VERTEX 0x1000

static int	numShadowIndices;
static unsigned short shadowIndices[MAX_SHADOW_INDEXES];

static int remap[MAX_VERTEX];
static int numFacePlane;
static int facing[MAX_FACE_PLANE];

static int	numShadowVerts;
static Vec3	shadowVerts[MAX_SHADOW_VERTS];
static bool overflowed;
static int c_sils;

#define DotProduct( a, b)			((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VectorSubtract( a, b, c )	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])

void R_GlobalPointToLocal( const float modelMatrix[16], const Vec3 &in, Vec3 &out ) {
	Vec3	temp;

	VectorSubtract( in, &modelMatrix[12], temp );

	out[0] = DotProduct( temp, &modelMatrix[0] );
	out[1] = DotProduct( temp, &modelMatrix[4] );
	out[2] = DotProduct( temp, &modelMatrix[8] );
}

srfTriangles_t* R_GenerateCaps( Vec3 lightPos, srfTriangles_t* tri )
{
	if (!tri->facePlanes || !tri->facePlanesCalculated)
		R_DeriveFacePlanes(tri);

	int numPlanes = tri->numIndexes / 3; 
	Plane* plane = tri->facePlanes;
	for (int i=0; i<numPlanes; i++)
	{
		if(plane[i].Distance(lightPos) > 0)
			facing[numPlanes++] = i;
	}

	srfTriangles_t* tris = R_AllocStaticTriSurf();
	R_AllocStaticTriSurfVerts(tri, numPlanes * 3);

	return NULL;
}

/*
=================
R_AddSilEdges

Add quads from the front points to the projected points
for each silhouette edge in the light
=================
*/
static void R_AddSilEdges( const srfTriangles_t *tri ) {
	int		v1, v2;
	int		i;
	silEdge_t	*sil;
	int		numPlanes;

	numPlanes = tri->numIndexes / 3;

	// add sil edges for any true silhouette boundaries on the surface
	for ( i = 0 ; i < tri->numSilEdges ; i++ ) {
		sil = tri->silEdges + i;
		if ( sil->p1 < 0 || sil->p1 > numPlanes || sil->p2 < 0 || sil->p2 > numPlanes ) {
			Sys_Error( "Bad sil planes" );
		}

		// an edge will be a silhouette edge if the face on one side
		// casts a shadow, but the face on the other side doesn't.
		// "casts a shadow" means that it has some surface in the projection,
		// not just that it has the correct facing direction
		// This will cause edges that are exactly on the frustum plane
		// to be considered sil edges if the face inside casts a shadow.
		if (facing[ sil->p1] == facing[ sil->p2 ]) {
			//Sys_Printf("remove %d %d\n", sil->v1, sil->v2);
			continue;
		}

		// don't overflow
		if ( numShadowIndices + 6 > MAX_SHADOW_INDEXES ) {
			overflowed = true;
			return;
		}

		v1 = remap[ sil->v1 ];
		v2 = remap[ sil->v2 ];

		// we need to choose the correct way of triangulating the silhouette quad
		// consistantly between any two points, no matter which order they are specified.
		// If this wasn't done, slight rasterization cracks would show in the shadow
		// volume when two sil edges were exactly coincident
		if ( facing[ sil->p2 ] ) {
			shadowIndices[numShadowIndices++] = v1;
			shadowIndices[numShadowIndices++] = v1+1;
			shadowIndices[numShadowIndices++] = v2;
			shadowIndices[numShadowIndices++] = v2;
			shadowIndices[numShadowIndices++] = v1+1;
			shadowIndices[numShadowIndices++] = v2+1;
		} else { 
			shadowIndices[numShadowIndices++] = v1;
			shadowIndices[numShadowIndices++] = v2;
			shadowIndices[numShadowIndices++] = v1+1;
			shadowIndices[numShadowIndices++] = v2;
			shadowIndices[numShadowIndices++] = v2+1;
			shadowIndices[numShadowIndices++] = v1+1;
		}
	}
}

/*
===================
R_ProjectPointsToFarPlane

make a projected copy of the even verts into the odd spots
that is on the far light clip plane
===================
*/
static void R_ProjectPointsToFarPlane( Vec3& lightPos, mat4& modelMatrix, int firstShadowVert, int numShadowVerts ) {
	Vec3		lv;
	Vec3	*in;

	R_GlobalPointToLocal( modelMatrix.m, lightPos, lv );

	// make a projected copy of the even verts into the odd spots
	in = &shadowVerts[firstShadowVert];
	for (int i = firstShadowVert ; i < numShadowVerts ; i+= 2, in += 2 ) {
		in[1] = in[0] + (in[0] - lv).normalize()*100;
	}
}

void Interaction::CreateInteraction( srfTriangles_t* tri, Vec3& lightPos, mat4& modelMatrix )
{
	if (tri->calcShadow)
		return;

	if (!tri->facePlanes || !tri->facePlanesCalculated)
		R_DeriveFacePlanes(tri);

	Vec3 lv;
	R_GlobalPointToLocal( modelMatrix.m, lightPos, lv );
	int numPlanes = tri->numIndexes / 3; 
	Plane* plane = tri->facePlanes;
	
	for (int i=0; i<numPlanes; i++)
	{
		facing[i] = plane[i].Distance(lv) > 0;
		//Sys_Printf("%f %f %f %d\n", plane[i][0], plane[i][1], plane[i][2], facing[i]);
	}

	memset(remap, -1, sizeof(remap));
	numShadowVerts = 0;
	int numTris = tri->numIndexes / 3;
	for ( int i = 0 ; i < numTris ; i++ ) {
		int		i1, i2, i3;

		// if it isn't facing the right way, don't add it        
		// to the shadow volume
		if ( facing[i] ) {
			continue;
		}

		i1 = tri->indices[ i*3 + 0 ];
		i2 = tri->indices[ i*3 + 1 ];
		i3 = tri->indices[ i*3 + 2 ];

		// make sure the verts that are not on the negative sides
		// of the frustum are copied over.
		// we need to get the original verts even from clipped triangles
		// so the edges reference correctly, because an edge may be unclipped
		// even when a triangle is clipped.
		if ( numShadowVerts + 6 > MAX_SHADOW_VERTS ) {
			overflowed = true;
			return;
		}

		if ( remap[i1] == -1 ) {
			remap[i1] = numShadowVerts;
			shadowVerts[ numShadowVerts ] = tri->verts[i1].xyz;
			numShadowVerts+=2;
		}
		if ( remap[i2] == -1 ) {
			remap[i2] = numShadowVerts;
			shadowVerts[ numShadowVerts ] = tri->verts[i2].xyz;
			numShadowVerts+=2;
		}
		if ( remap[i3] == -1 ) {
			remap[i3] = numShadowVerts;
			shadowVerts[ numShadowVerts ] = tri->verts[i3].xyz;
			numShadowVerts+=2;
		}

		// instead of overflowing or drawing a streamer shadow, don't draw a shadow at all
		if ( numShadowIndices + 3 > MAX_SHADOW_INDEXES ) {
			overflowed = true;
			return;
		}
		if ( remap[i1] == -1 || remap[i2] == -1 || remap[i3] == -1 ) {
			Sys_Error( "R_CreateShadowVolumeInFrustum: bad remap[]" );
		}

		shadowIndices[numShadowIndices++] = remap[i1];
		shadowIndices[numShadowIndices++] = remap[i2];
		shadowIndices[numShadowIndices++] = remap[i3];
	}

	Sys_Printf("front face %d\n", numShadowIndices/3);
	int preSilIndexes = numShadowIndices;
	// any edges that are a transition between a shadowing and
	// non-shadowing triangle will cast a silhouette edge
	R_AddSilEdges( tri );

	c_sils += numShadowIndices - preSilIndexes;

	int firstShadowVert = 0;

	// project all of the vertexes to the shadow plane, generating
	// an equal number of back vertexes
	R_ProjectPointsToFarPlane(lightPos, modelMatrix, firstShadowVert, numShadowVerts );
	
	shadowTris->numVerts = numShadowVerts;
	shadowTris->numIndexes = numShadowIndices;
	R_AllocStaticTriSurfVerts(shadowTris, shadowTris->numVerts);
	R_AllocStaticTriSurfIndices(shadowTris, numShadowIndices);

	for (int i=0; i<numShadowVerts; ++i)
	{
		shadowTris->verts[i].xyz = shadowVerts[i];
		//Sys_Printf("%f %f %f\n", shadowTris->verts[i].xyz.x, shadowTris->verts[i].xyz.y, shadowTris->verts[i].xyz.y);
	}

	for (int i=0; i<shadowTris->numIndexes; ++i)
		shadowTris->indices[i] = shadowIndices[i];

	R_GenerateGeometryVbo(shadowTris);

	tri->calcShadow = true;
}

Interaction::Interaction()
{
	shadowTris = R_AllocStaticTriSurf();
}

Interaction::~Interaction()
{

}
