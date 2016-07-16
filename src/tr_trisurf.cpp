#include "r_public.h"
#include "common/Plane.h"
#include "common/mem.h"
#include "sys/sys_public.h"

const int MAX_SIL_EDGES			= 0x10000;
const int SILEDGE_HASH_SIZE		= 1024;
static int			numSilEdges;
static silEdge_t	silEdges[MAX_SIL_EDGES];
static int			numPlanes;
static glIndex_t sigIndice[MAX_SIL_EDGES];
/*
==================
R_DeriveTangents

This is called once for static surfaces, and every frame for deforming surfaces

Builds tangents, normals, and face planes
==================
*/
void R_DeriveNormals( srfTriangles_t *tri ) {
	int				i;

	if ( tri->tangentsCalculated ) {
		return;
	}

	for ( i = 0; i < tri->numVerts; i++ ) {
		tri->verts[i].normal.Zero();
		tri->verts[i].tangents[0].Zero();
		tri->verts[i].tangents[1].Zero();
	}

	for ( i = 0; i < tri->numIndices; i += 3 ) {
		// make face tangents
		float		d0[5], d1[5];
		DrawVert	*a, *b, *c;
		Vec3		temp, normal, tangents[2];

		a = tri->verts + tri->indices[i + 0];
		b = tri->verts + tri->indices[i + 1];
		c = tri->verts + tri->indices[i + 2];

		d0[0] = b->xyz[0] - a->xyz[0];
		d0[1] = b->xyz[1] - a->xyz[1];
		d0[2] = b->xyz[2] - a->xyz[2];
		d0[3] = b->st[0] - a->st[0];
		d0[4] = b->st[1] - a->st[1];

		d1[0] = c->xyz[0] - a->xyz[0];
		d1[1] = c->xyz[1] - a->xyz[1];
		d1[2] = c->xyz[2] - a->xyz[2];
		d1[3] = c->st[0] - a->st[0];
		d1[4] = c->st[1] - a->st[1];

		// normal
		temp[0] = d1[1] * d0[2] - d1[2] * d0[1];
		temp[1] = d1[2] * d0[0] - d1[0] * d0[2];
		temp[2] = d1[0] * d0[1] - d1[1] * d0[0];
		normal.set(temp.normalize());

		temp[0] = (d0[0] * d1[4] - d0[4] * d1[0]);
		temp[1] = (d0[1] * d1[4] - d0[4] * d1[1]);
		temp[2] = (d0[2] * d1[4] - d0[4] * d1[2]);
		tangents[0].set(temp.normalize());

		temp[0] = (d0[3] * d1[0] - d0[0] * d1[3]);
		temp[1] = (d0[3] * d1[1] - d0[1] * d1[3]);
		temp[2] = (d0[3] * d1[2] - d0[2] * d1[3]);
		tangents[1].set(temp.normalize());

		// sum up the tangents and normals for each vertex on this face
		for ( int j = 0 ; j < 3 ; j++ ) {
			DrawVert* vert = &tri->verts[tri->indices[i+j]];
			vert->normal += normal;
			vert->tangents[0] += tangents[0];
			vert->tangents[1] += tangents[1];
		}
	}

	tri->tangentsCalculated = true;
}

/*
=================
SilEdgeSort
=================
*/
static int SilEdgeSort( const void *a, const void *b ) {
	if ( ((silEdge_t *)a)->p1 < ((silEdge_t *)b)->p1 ) {
		return -1;
	}
	if ( ((silEdge_t *)a)->p1 > ((silEdge_t *)b)->p1 ) {
		return 1;
	}
	if ( ((silEdge_t *)a)->p2 < ((silEdge_t *)b)->p2 ) {
		return -1;
	}
	if ( ((silEdge_t *)a)->p2 > ((silEdge_t *)b)->p2 ) {
		return 1;
	}
	return 0;
}

/*
=================
R_MergeSurfaceList

Only deals with vertexes and indexes, not silhouettes, planes, etc.
Does NOT perform a cleanup triangles, so there may be duplicated verts in the result.
=================
*/
srfTriangles_t	*R_MergeSurfaceList( const srfTriangles_t **surfaces, int numSurfaces ) {
	srfTriangles_t	*newTri;
	const srfTriangles_t	*tri;
	int				i, j;
	int				totalVerts;
	int				totalIndexes;

	totalVerts = 0;
	totalIndexes = 0;
	for ( i = 0 ; i < numSurfaces ; i++ ) {
		totalVerts += surfaces[i]->numVerts;
		totalIndexes += surfaces[i]->numIndices;
	}

	newTri = R_AllocStaticTriSurf();
	newTri->numVerts = totalVerts;
	newTri->numIndices = totalIndexes;
	R_AllocStaticTriSurfVerts( newTri, newTri->numVerts );
	R_AllocStaticTriSurfIndices( newTri, newTri->numIndices );

	totalVerts = 0;
	totalIndexes = 0;
	for ( i = 0 ; i < numSurfaces ; i++ ) {
		tri = surfaces[i];
		memcpy( newTri->verts + totalVerts, tri->verts, tri->numVerts * sizeof( *tri->verts ) );
		for ( j = 0 ; j < tri->numIndices ; j++ ) {
			newTri->indices[ totalIndexes + j ] = totalVerts + tri->indices[j];
		}
		totalVerts += tri->numVerts;
		totalIndexes += tri->numIndices;
	}

	return newTri;
}

/*
=================
R_MergeTriangles

Only deals with vertexes and indexes, not silhouettes, planes, etc.
Does NOT perform a cleanup triangles, so there may be duplicated verts in the result.
=================
*/
srfTriangles_t	*R_MergeTriangles( const srfTriangles_t *tri1, const srfTriangles_t *tri2 ) {
	const srfTriangles_t	*tris[2];

	tris[0] = tri1;
	tris[1] = tri2;

	return R_MergeSurfaceList( tris, 2 );
}

/*
=================
R_AllocStaticTriSurfIndexes
=================
*/
void R_AllocStaticTriSurfIndices( srfTriangles_t *tri, int numIndexes ) {
	if (tri->indices)
		delete[] tri->indices;

	tri->indices = new glIndex_t[numIndexes];
}

void R_DeriveFacePlanes( srfTriangles_t *tri ) {
	Plane *	planes;

	if ( !tri->facePlanes ) {
		R_AllocStaticTriSurfPlanes( tri, tri->numIndices );
	}
	planes = tri->facePlanes;

	for ( int i = 0; i < tri->numIndices; i+= 3, planes++ ) {
		int		i1, i2, i3;
		Vec3	d1, d2, normal;
		Vec3	*v1, *v2, *v3;

		i1 = tri->indices[i + 0];
		i2 = tri->indices[i + 1];
		i3 = tri->indices[i + 2];

		v1 = &tri->verts[i1].xyz;
		v2 = &tri->verts[i2].xyz;
		v3 = &tri->verts[i3].xyz;

		d1[0] = v2->x - v1->x;
		d1[1] = v2->y - v1->y;
		d1[2] = v2->z - v1->z;

		d2[0] = v3->x - v1->x;
		d2[1] = v3->y - v1->y;
		d2[2] = v3->z - v1->z;

		normal[0] = d2.y * d1.z - d2.z * d1.y;
		normal[1] = d2.z * d1.x - d2.x * d1.z;
		normal[2] = d2.x * d1.y - d2.y * d1.x;

		float sqrLength, invLength;

		sqrLength = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
		invLength = idMath::RSqrt( sqrLength );

		(*planes)[0] = normal[0] * invLength;
		(*planes)[1] = normal[1] * invLength;
		(*planes)[2] = normal[2] * invLength;

		planes->FitThroughPoint( *v1 );
	}

	tri->facePlanesCalculated = true;
}

/*
===============
R_DefineEdge
===============
*/
static int c_duplicatedEdges, c_tripledEdges;

static void R_DefineEdge( int v1, int v2, int planeNum ) {
	int		i;

	// check for degenerate edge
	if ( v1 == v2 ) {
		return;
	}
	// search for a matching other side
	for ( i = 0; i<numSilEdges; ++i) {
		if ( silEdges[i].v1 == v1 && silEdges[i].v2 == v2 ) {
			c_duplicatedEdges++;
			// allow it to still create a new edge
			continue;
		}
		if ( silEdges[i].v2 == v1 && silEdges[i].v1 == v2 ) {
			if ( silEdges[i].p2 != numPlanes )  {
				c_tripledEdges++;
				// allow it to still create a new edge
				continue;
			}
			// this is a matching back side
			silEdges[i].p2 = planeNum;
			return;
		}

	}

	// define the new edge
	if ( numSilEdges == MAX_SIL_EDGES ) {
		Sys_Warning( "MAX_SIL_EDGES" );
		return;
	}
	
	silEdges[numSilEdges].p1 = planeNum;
	silEdges[numSilEdges].p2 = numPlanes;
	silEdges[numSilEdges].v1 = v1;
	silEdges[numSilEdges].v2 = v2;

	numSilEdges++;
}

/*
=================
R_IdentifySilEdges

If the surface will not deform, coplanar edges (polygon interiors)
can never create silhouette plains, and can be omited
=================
*/
int	c_coplanarSilEdges;
int	c_totalSilEdges;

void R_IdentifySilEdges( srfTriangles_t *tri ) {
	int		i;
	int		numTris;
	int		shared, single;

	numTris = tri->numIndices / 3;

	numSilEdges = 0;
	numPlanes = numTris;

	c_duplicatedEdges = 0;
	c_tripledEdges = 0;

	for ( i = 0 ; i < numTris ; i++ ) {
		int		i1, i2, i3;

		i1 = tri->silIndices[ i*3 + 0 ];
		i2 = tri->silIndices[ i*3 + 1 ];
		i3 = tri->silIndices[ i*3 + 2 ];

		// create the edges
		R_DefineEdge( i1, i2, i );
		R_DefineEdge( i2, i3, i );
		R_DefineEdge( i3, i1, i );
	}

	if ( c_duplicatedEdges || c_tripledEdges ) {
		Sys_Warning( "%i duplicated edge directions, %i tripled edges", c_duplicatedEdges, c_tripledEdges );
	}

	// if we know that the vertexes aren't going
	// to deform, we can remove interior triangulation edges
	// on otherwise planar polygons.
	// I earlier believed that I could also remove concave
	// edges, because they are never silhouettes in the conventional sense,
	// but they are still needed to balance out all the true sil edges
	// for the shadow algorithm to function
	int		c_coplanarCulled;

	c_coplanarCulled = 0;
	c_totalSilEdges += numSilEdges;

	// sort the sil edges based on plane number
	qsort( silEdges, numSilEdges, sizeof( silEdges[0] ), SilEdgeSort );

	// count up the distribution.
	// a perfectly built model should only have shared
	// edges, but most models will have some interpenetration
	// and dangling edges
	shared = 0;
	single = 0;
	for ( i = 0 ; i < numSilEdges ; i++ ) {
		if ( silEdges[i].p2 == numPlanes ) {
			sigIndice[single*2] = silEdges[i].v1;
			sigIndice[single*2 + 1] = silEdges[i].v2;
			single++;
		} else {
			shared++;
		}
	}

	if ( !single ) {
		tri->perfectHull = true;
	} else {
		tri->perfectHull = false;
		Sys_Warning("built model has single edges %d\n", single);
	}

	tri->numSilEdges = numSilEdges;
	tri->silEdges = (silEdge_t*)mem_alloc( numSilEdges * sizeof(silEdge_t) );
	memcpy( tri->silEdges, silEdges, numSilEdges * sizeof( tri->silEdges[0] ) );

}

