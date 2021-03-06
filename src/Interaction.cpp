#include "Interaction.h"
#include "common/Plane.h"
#include "r_public.h"
#include "sys/sys_public.h"
#

#define	MAX_SHADOW_INDEXES		0x18000
#define	MAX_SHADOW_VERTS		0x18000
#define MAX_FACE_PLANE 0x1000
#define MAX_VERTEX 0x1000

static int	numShadowIndices;
static unsigned short shadowIndices[MAX_SHADOW_INDEXES];

static int MaxUShort = 65535;

static unsigned short remap[MAX_VERTEX];
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

	int numPlanes = tri->numIndices / 3; 
	Plane* plane = tri->facePlanes;
	for (int i=0; i<numPlanes; i++) {
		if(plane[i].Distance(lightPos) > 0)
			facing[numPlanes++] = i;
	}

	srfTriangles_t* tris = R_AllocStaticTriSurf();
	R_AllocStaticTriSurfVerts(tri, numPlanes * 3);

	return NULL;
}

/*
===============
PointsOrdered

To make sure the triangulations of the sil edges is consistant,
we need to be able to order two points.  We don't care about how
they compare with any other points, just that when the same two
points are passed in (in either order), they will always specify
the same one as leading.

Currently we need to have separate faces in different surfaces
order the same way, so we must look at the actual coordinates.
If surfaces are ever guaranteed to not have to edge match with
other surfaces, we could just compare indexes.
===============
*/
static bool PointsOrdered( const Vec3 &a, const Vec3 &b ) {
	float	i, j;

	// vectors that wind up getting an equal hash value will
	// potentially cause a misorder, which can show as a couple
	// crack pixels in a shadow

	// scale by some odd numbers so -8, 8, 8 will not be equal
	// to 8, -8, 8

	// in the very rare case that these might be equal, all that would
	// happen is an oportunity for a tiny rasterization shadow crack
	i = a[0] + a[1]*127 + a[2]*1023;
	j = b[0] + b[1]*127 + b[2]*1023;

	return (bool)(i < j);
}


/*
=================
R_AddSilEdges

Add quads from the front points to the projected points
for each silhouette edge in the light
=================
*/
static void R_AddSilEdges( const srfTriangles_t *tri ) {
	glIndex_t		v1, v2;
	int		i;
	silEdge_t	*sil;
	int		numPlanes;

	numPlanes = tri->numIndices / 3;

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

		if (v1 == MaxUShort || v2 == MaxUShort)
		{
			// Sys_Printf("max unsigned short %d %d", sil->v1, v1);
			continue;
		}

		// we need to choose the correct way of triangulating the silhouette quad
		// consistantly between any two points, no matter which order they are specified.
		// If this wasn't done, slight rasterization cracks would show in the shadow
		// volume when two sil edges were exactly coincident
		if ( facing[ sil->p2 ] ) {
			if ( PointsOrdered( shadowVerts[ v1 ], shadowVerts[ v2 ] ) ) {
				shadowIndices[numShadowIndices++] = v1;
				shadowIndices[numShadowIndices++] = v1+1;
				shadowIndices[numShadowIndices++] = v2;
				shadowIndices[numShadowIndices++] = v2;
				shadowIndices[numShadowIndices++] = v1+1;
				shadowIndices[numShadowIndices++] = v2+1;
				//Sys_Printf("%d %d %d %d %d %d\n ", v1, v1+1, v2, v2, v1 + 1, v2+1);
				} else {
				shadowIndices[numShadowIndices++] = v1;
				shadowIndices[numShadowIndices++] = v2+1;
				shadowIndices[numShadowIndices++] = v2;
				shadowIndices[numShadowIndices++] = v1;
				shadowIndices[numShadowIndices++] = v1+1;
				shadowIndices[numShadowIndices++] = v2+1;
				}
		} else { 
			if ( PointsOrdered( shadowVerts[ v1 ], shadowVerts[ v2 ] ) ) {
				shadowIndices[numShadowIndices++] = v1;
				shadowIndices[numShadowIndices++] = v2;
				shadowIndices[numShadowIndices++] = v1+1;
				shadowIndices[numShadowIndices++] = v2;
				shadowIndices[numShadowIndices++] = v2+1;
				shadowIndices[numShadowIndices++] = v1+1;
				//Sys_Printf("%d %d %d %d %d %d\n ", v1, v2, v1+1, v2, v2 + 1, v1+1);
			} else {
				shadowIndices[numShadowIndices++] = v1;
				shadowIndices[numShadowIndices++] = v2;
				shadowIndices[numShadowIndices++] = v2+1;
				shadowIndices[numShadowIndices++] = v1;
				shadowIndices[numShadowIndices++] = v2+1;
				shadowIndices[numShadowIndices++] = v1+1;
			}
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
	//	in[1] = lightPos;
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
	int numPlanes = tri->numIndices / 3; 
	Plane* plane = tri->facePlanes;
	
	for (int i=0; i<numPlanes; i++) {
		facing[i] = plane[i].Distance(lv) > 0.f;
		//Sys_Printf("%f %f %f %d\n", plane[i][0], plane[i][1], plane[i][2], facing[i]);
	}

	// for dangling edges to reference
	facing[numPlanes] = 1;

	memset(remap, MaxUShort, sizeof(remap));
	numShadowVerts = 0;
	int numTris = tri->numIndices / 3;
	for ( int i = 0 ; i < numTris ; i++ ) {
		int		i1, i2, i3;

		// if it isn't facing the right way, don't add it        
		// to the shadow volume
		if ( facing[i] ) {
			continue;
		}

		i1 = tri->silIndices[ i*3 + 0 ];
		i2 = tri->silIndices[ i*3 + 1 ];
		i3 = tri->silIndices[ i*3 + 2 ];

		// make sure the verts that are not on the negative sides
		// of the frustum are copied over.
		// we need to get the original verts even from clipped triangles
		// so the edges reference correctly, because an edge may be unclipped
		// even when a triangle is clipped.
		if ( numShadowVerts + 6 > MAX_SHADOW_VERTS ) {
			overflowed = true;
			return;
		}

		// i1��û�м�¼
		if ( remap[i1] == MaxUShort ) {
			remap[i1] = numShadowVerts;
			shadowVerts[ numShadowVerts ] = tri->verts[i1].xyz;
			numShadowVerts+=2;
		}
		if ( remap[i2] == MaxUShort ) {
			remap[i2] = numShadowVerts;
			shadowVerts[ numShadowVerts ] = tri->verts[i2].xyz;
			numShadowVerts+=2;
		}
		if ( remap[i3] == MaxUShort ) {
			remap[i3] = numShadowVerts;
			shadowVerts[ numShadowVerts ] = tri->verts[i3].xyz;
			numShadowVerts+=2;
		}

		// instead of overflowing or drawing a streamer shadow, don't draw a shadow at all
		if ( numShadowIndices + 3 > MAX_SHADOW_INDEXES ) {
			overflowed = true;
			return;
		}
		if ( remap[i1] == MaxUShort || remap[i2] == MaxUShort || remap[i3] == MaxUShort ) {
			Sys_Error( "R_CreateShadowVolumeInFrustum: bad remap[]" );
		}

		shadowIndices[numShadowIndices++] = remap[i3];
		shadowIndices[numShadowIndices++] = remap[i2];
		shadowIndices[numShadowIndices++] = remap[i1];

		/*Sys_Printf("shadow indices %d %d %d %d\n", numShadowIndices, shadowIndices[numShadowIndices-3], 
			shadowIndices[numShadowIndices-2], shadowIndices[numShadowIndices-1]);*/
	}

	numCapIndices = numShadowIndices;

	//for (int i = 0; i < numCapIndices; i += 3) 
	//{
	//	shadowIndices[numShadowIndices + i + 0] = shadowIndices[ i + 2] + 1;
	//	shadowIndices[numShadowIndices + i + 1] = shadowIndices[ i + 1] + 1;
	//	shadowIndices[numShadowIndices + i + 2] = shadowIndices[ i + 0] + 1;
	//}
	//numShadowIndices += numCapIndices;

	Sys_Printf("front face %d\n", numShadowIndices/3);
	int preSilIndexes = numShadowIndices;
	numCapIndices = numShadowIndices;
	// any edges that are a transition between a shadowing and
	// non-shadowing triangle will cast a silhouette edge
	R_AddSilEdges( tri );

	c_sils += numShadowIndices - preSilIndexes;
	numSilIndices = c_sils;

	int firstShadowVert = 0;

	// project all of the vertexes to the shadow plane, generating
	// an equal number of back vertexes
	R_ProjectPointsToFarPlane(lightPos, modelMatrix, firstShadowVert, numShadowVerts );
	
	shadowTris->numVerts = numShadowVerts;
	shadowTris->numIndices = numShadowIndices;
	R_AllocStaticTriSurfVerts(shadowTris, shadowTris->numVerts);
	R_AllocStaticTriSurfIndices(shadowTris, numShadowIndices);

	for (int i=0; i<numShadowVerts; ++i) {
		shadowTris->verts[i].xyz = shadowVerts[i];
		shadowTris->verts[i].color[3] = i%2==0 ? 255 : 0;
		shadowTris->verts[i].color[0] = i%2==0 ? 255 : 0;
		shadowTris->verts[i].color[1] = 0;
		shadowTris->verts[i].color[2] = 0;
		//Sys_Printf("%f %f %f\n", shadowTris->verts[i].xyz.x, shadowTris->verts[i].xyz.y, shadowTris->verts[i].xyz.y);
	}

	for (int i=0; i<shadowTris->numIndices; ++i) {
		//if (shadowIndices[i] == 65535)
		{
			//Sys_Printf("shadowtris %d %u\n", i, shadowIndices[i]);
		}
		shadowTris->indices[i] = shadowIndices[i];
	}

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
