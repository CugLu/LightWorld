#include "r_public.h"
#include "common/Plane.h"
#include "common/mem.h"
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

	for ( i = 0; i < tri->numIndexes; i += 3 ) {
		// make face tangents
		float		d0[5], d1[5];
		DrawVert	*a, *b, *c;
		Vec3		temp, normal, tangents[2];

		a = tri->verts + tri->indexes[i + 0];
		b = tri->verts + tri->indexes[i + 1];
		c = tri->verts + tri->indexes[i + 2];

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
			DrawVert* vert = &tri->verts[tri->indexes[i+j]];
			vert->normal += normal;
			vert->tangents[0] += tangents[0];
			vert->tangents[1] += tangents[1];
		}
	}

	tri->tangentsCalculated = true;
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
		totalIndexes += surfaces[i]->numIndexes;
	}

	newTri = R_AllocStaticTriSurf();
	newTri->numVerts = totalVerts;
	newTri->numIndexes = totalIndexes;
	R_AllocStaticTriSurfVerts( newTri, newTri->numVerts );
	R_AllocStaticTriSurfIndexes( newTri, newTri->numIndexes );

	totalVerts = 0;
	totalIndexes = 0;
	for ( i = 0 ; i < numSurfaces ; i++ ) {
		tri = surfaces[i];
		memcpy( newTri->verts + totalVerts, tri->verts, tri->numVerts * sizeof( *tri->verts ) );
		for ( j = 0 ; j < tri->numIndexes ; j++ ) {
			newTri->indexes[ totalIndexes + j ] = totalVerts + tri->indexes[j];
		}
		totalVerts += tri->numVerts;
		totalIndexes += tri->numIndexes;
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
void R_AllocStaticTriSurfIndexes( srfTriangles_t *tri, int numIndexes ) {
	assert( tri->indexes == NULL );
	tri->indexes = new glIndex_t[numIndexes];
}

/*
=================
R_StaticFree
=================
*/
void R_StaticFree( void *data ) {
	//tr.pc.c_free++;
    mem_free( data );
}

/*
==============
R_FreeStaticTriSurf

This will defer the free until the current frame has run through the back end.
==============
*/
void R_FreeStaticTriSurf( srfTriangles_t *tri ) {
	delete tri;
//	frameData_t		*frame;
//
//	if ( !tri ) {
//		return;
//	}
//
//	if ( tri->nextDeferredFree ) {
//		common->Error( "R_FreeStaticTriSurf: freed a freed triangle" );
//	}
//	frame = frameData;
//
//	if ( !frame ) {
//		// command line utility, or rendering in editor preview mode ( force )
//		R_ReallyFreeStaticTriSurf( tri );
//	} else {
//#ifdef ID_DEBUG_MEMORY
//		R_CheckStaticTriSurfMemory( tri );
//#endif
//		tri->nextDeferredFree = NULL;
//		if ( frame->lastDeferredFreeTriSurf ) {
//			frame->lastDeferredFreeTriSurf->nextDeferredFree = tri;
//		} else {
//			frame->firstDeferredFreeTriSurf = tri;
//		}
//		frame->lastDeferredFreeTriSurf = tri;
//	}
}

/*
=================
R_StaticAlloc
=================
*/
void *R_StaticAlloc( int bytes ) {
	void	*buf;

	//tr.pc.c_alloc++;

	//tr.staticAllocCount += bytes;

    buf = mem_alloc( bytes );

	//// don't exit on failure on zero length allocations since the old code didn't
	//if ( !buf && ( bytes != 0 ) ) {
	//	common->FatalError( "R_StaticAlloc failed on %i bytes", bytes );
	//}
	return buf;
}

/*
=================
R_ClearedStaticAlloc
=================
*/
void *R_ClearedStaticAlloc( int bytes ) {
	void	*buf;

	buf = R_StaticAlloc( bytes );
	memset(buf, 0, bytes);
	//SIMDProcessor->Memset( buf, 0, bytes );
	return buf;
}