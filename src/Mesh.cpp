#include "sys/sys_public.h"
#include "Mesh.h"
#include "MeshLoaderLwo.h"
#include "common/mem.h"

Mesh::Mesh() 
{

}

Mesh::~Mesh()
{

}

srfTriangles_t* Mesh::AllocGeo()
{
	srfTriangles_t* geo = R_AllocStaticTriSurf();
	_geometries.push_back(geo);
	return geo;
}

void Mesh::GenerateNormals()
{
	for (unsigned int i=0; i<_geometries.size(); ++i)
	{
		R_DeriveNormals(_geometries[i]);
	}
}

void Mesh::CalcBounds()
{
	for (unsigned int i=0; i<_geometries.size(); ++i)
	{
		R_BoundTriSurf(_geometries[i]);
	}
}

#include "MeshLoaderLwo.h"
typedef struct matchVert_s {
	struct matchVert_s	*next;
	int		v, tv;
	byte	color[4];
	Vec3	normal;
} matchVert_t;

bool Mesh::ConvertLWOToModelSurfaces( const struct st_lwObject *lwo ) {
	srfTriangles_t *tri;
	int	numTVertexes;
	int	i, j, k;
	int	v, tv;
	Vec3* vList;
	int *			vRemap;
	Vec2 *		tvList;
	int *			tvRemap;
	matchVert_t *	mvTable;	// all of the match verts
	matchVert_t **	mvHash;		// points inside mvTable for each xyz index
	matchVert_t *	lastmv;
	matchVert_t *	mv;
	Vec3			normal;
	byte			color[4];
	drawSurf_t* surf;

	if ( !lwo ) {
		return false;
	}
	if ( lwo->surf == NULL ) {
		return false;
	}

	//timeStamp = lwo->timeStamp;

	// count the number of surfaces
	lwSurface *		lwoSurf;
	i = 0;
	for ( lwoSurf = lwo->surf; lwoSurf; lwoSurf = lwoSurf->next ) {
		i++;
	}

	// the modeling programs can save out multiple surfaces with a common
	// material, but we would like to merge them together where possible
	memset( &surf, 0, sizeof( surf ) );

	for ( lwoSurf = lwo->surf, i = 0; lwoSurf; lwoSurf = lwoSurf->next, i++ ) 
	{
		Sys_Printf("lwo surface name %s\n", lwoSurf->name);
	}

	//idVectorSubset<idVec3, 3> vertexSubset;
	//idVectorSubset<idVec2, 2> texCoordSubset;

	// we only ever use the first layer
	lwLayer *layer = lwo->layer;

	// vertex positions
	if ( layer->point.count <= 0 ) {
		Sys_Warning( "ConvertLWOToModelSurfaces: model \'%s\' has bad or missing vertex data" );
		return false;
	}

	vList = (Vec3 *)R_StaticAlloc( layer->point.count * sizeof( vList[0] ) );
	for ( j = 0; j < layer->point.count; j++ ) {
		vList[j].x = layer->point.pt[j].pos[0];
		vList[j].y = layer->point.pt[j].pos[2];
		vList[j].z = layer->point.pt[j].pos[1];
	}

	// vertex texture coords
	numTVertexes = 0;

	if ( layer->nvmaps ) {
		for( lwVMap *vm = layer->vmap; vm; vm = vm->next ) {
			if ( vm->type == LWID_('T','X','U','V') ) {
				numTVertexes += vm->nverts;
			}
		}
	}

	if ( numTVertexes ) {
		tvList = (Vec2 *)mem_alloc( numTVertexes * sizeof( tvList[0] ) );
		int offset = 0;
		for( lwVMap *vm = layer->vmap; vm; vm = vm->next ) {
			if ( vm->type == LWID_('T','X','U','V') ) {
	  			vm->offset = offset;
		  		for ( k = 0; k < vm->nverts; k++ ) {
		  		   	tvList[k + offset].x = vm->val[k][0];
					tvList[k + offset].y = 1.0f - vm->val[k][1];	// invert the t
		  		}
			  	offset += vm->nverts;
			}
		}
	} else {
		Sys_Warning( "ConvertLWOToModelSurfaces: model  has bad or missing uv data" );
	  	numTVertexes = 1;
		tvList = (Vec2 *)mem_cleared_alloc( numTVertexes * sizeof( tvList[0] ) );
	}

	// It seems like the tools our artists are using often generate
	// verts and texcoords slightly separated that should be merged
	// note that we really should combine the surfaces with common materials
	// before doing this operation, because we can miss a slop combination
	// if they are in different surfaces

	vRemap = (int *)R_StaticAlloc( layer->point.count * sizeof( vRemap[0] ) );

	//if ( fastLoad ) {
	//	// renderbump doesn't care about vertex count
	for ( j = 0; j < layer->point.count; j++ ) {
		vRemap[j] = j;
	}
	//} else {
	//	float vertexEpsilon = r_slopVertex.GetFloat();
	//	float expand = 2 * 32 * vertexEpsilon;
	//	idVec3 mins, maxs;

	//	SIMDProcessor->MinMax( mins, maxs, vList, layer->point.count );
	//	mins -= idVec3( expand, expand, expand );
	//	maxs += idVec3( expand, expand, expand );
	//	vertexSubset.Init( mins, maxs, 32, 1024 );
	//	for ( j = 0; j < layer->point.count; j++ ) {
	//		vRemap[j] = vertexSubset.FindVector( vList, j, vertexEpsilon );
	//	}
	//}

	tvRemap = (int *)R_StaticAlloc( numTVertexes * sizeof( tvRemap[0] ) );

	//if ( fastLoad ) {
	//	// renderbump doesn't care about vertex count
	for ( j = 0; j < numTVertexes; j++ ) {
		tvRemap[j] = j;
	}
	//} else {
	//	float texCoordEpsilon = r_slopTexCoord.GetFloat();
	//	float expand = 2 * 32 * texCoordEpsilon;
	//	idVec2 mins, maxs;

	//	SIMDProcessor->MinMax( mins, maxs, tvList, numTVertexes );
	//	mins -= idVec2( expand, expand );
	//	maxs += idVec2( expand, expand );
	//	texCoordSubset.Init( mins, maxs, 32, 1024 );
	//	for ( j = 0; j < numTVertexes; j++ ) {
	//		tvRemap[j] = texCoordSubset.FindVector( tvList, j, texCoordEpsilon );
	//	}
	//}

	// build the surfaces
	for ( lwoSurf = lwo->surf, i = 0; lwoSurf; lwoSurf = lwoSurf->next, i++ ) {
		//im1 = declManager->FindMaterial( lwoSurf->name );

		bool normalsParsed = true;

		// completely ignore any explict normals on surfaces with a renderbump command
		// which will guarantee the best contours and least vertexes.
		//const char *rb = im1->GetRenderBump();
		//if ( rb && rb[0] ) {
		//	normalsParsed = false;
		//}

		// we need to find out how many unique vertex / texcoord combinations there are

		// the maximum possible number of combined vertexes is the number of indexes
		mvTable = (matchVert_t *)R_ClearedStaticAlloc( layer->polygon.count * 3 * sizeof( mvTable[0] ) );

		// we will have a hash chain based on the xyz values
		mvHash = (matchVert_t **)R_ClearedStaticAlloc( layer->point.count * sizeof( mvHash[0] ) );

		// allocate triangle surface
		tri = AllocGeo();
		tri->numVerts = 0;
		tri->numIndexes = 0;
		R_AllocStaticTriSurfIndexes( tri, layer->polygon.count * 3 );
		tri->generateNormals = !normalsParsed;

		// find all the unique combinations
		float	normalEpsilon;
		//if ( fastLoad ) {
			normalEpsilon = 1.0f;	// don't merge unless completely exact
		//} else {
			//normalEpsilon = 1.0f - r_slopNormal.GetFloat();
		//}
		for ( j = 0; j < layer->polygon.count; j++ ) {
			lwPolygon *poly = &layer->polygon.pol[j];

			if ( poly->surf != lwoSurf ) {
				continue;
			}

			if ( poly->nverts != 3 ) {
				Sys_Warning( "ConvertLWOToModelSurfaces: model %s has too many verts for a poly! Make sure you triplet it down", name.c_str() );
				continue;
			}

			for ( k = 0; k < 3; k++ ) {

				v = vRemap[poly->v[k].index];

				normal.x = poly->v[k].norm[0];
				normal.y = poly->v[k].norm[2];
				normal.z = poly->v[k].norm[1];

				// LWO models aren't all that pretty when it comes down to the floating point values they store
				normal.FixDegenerateNormal();

				tv = 0;

				color[0] = lwoSurf->color.rgb[0] * 255;
				color[1] = lwoSurf->color.rgb[1] * 255;
				color[2] = lwoSurf->color.rgb[2] * 255;
				color[3] = 255;

				// first set attributes from the vertex
				lwPoint	*pt = &layer->point.pt[poly->v[k].index];
				int nvm;
				for ( nvm = 0; nvm < pt->nvmaps; nvm++ ) {
					lwVMapPt *vm = &pt->vm[nvm];

					if ( vm->vmap->type == LWID_('T','X','U','V') ) {
						tv = tvRemap[vm->index + vm->vmap->offset];
					}
					if ( vm->vmap->type == LWID_('R','G','B','A') ) {
						for ( int chan = 0; chan < 4; chan++ ) {
							color[chan] = 255 * vm->vmap->val[vm->index][chan];
						}
					}
				}

				// then override with polygon attributes
				for ( nvm = 0; nvm < poly->v[k].nvmaps; nvm++ ) {
					lwVMapPt *vm = &poly->v[k].vm[nvm];

					if ( vm->vmap->type == LWID_('T','X','U','V') ) {
						tv = tvRemap[vm->index + vm->vmap->offset];
					}
					if ( vm->vmap->type == LWID_('R','G','B','A') ) {
						for ( int chan = 0; chan < 4; chan++ ) {
							color[chan] = 255 * vm->vmap->val[vm->index][chan];
						}
					}
				}

				// find a matching vert
				for ( lastmv = NULL, mv = mvHash[v]; mv != NULL; lastmv = mv, mv = mv->next ) {
					if ( mv->tv != tv ) {
						continue;
					}
					if ( *(unsigned *)mv->color != *(unsigned *)color ) {
						continue;
					}
					if ( !normalsParsed ) {
						// if we are going to create the normals, just
						// matching texcoords is enough
						break;
					}
					if ( mv->normal.dot( normal ) > normalEpsilon ) {
						break;		// we already have this one
					}
				}
				if ( !mv ) {
					// allocate a new match vert and link to hash chain
					mv = &mvTable[ tri->numVerts ];
					mv->v = v;
					mv->tv = tv;
					mv->normal = normal;
					*(unsigned *)mv->color = *(unsigned *)color;
					mv->next = NULL;
					if ( lastmv ) {
						lastmv->next = mv;
					} else {
						mvHash[v] = mv;
					}
					tri->numVerts++;
				}

				tri->indexes[tri->numIndexes] = mv - mvTable;
				tri->numIndexes++;
			}
		}

		// allocate space for the indexes and copy them
		if ( tri->numIndexes > layer->polygon.count * 3 ) {
			Sys_Error( "ConvertLWOToModelSurfaces: index miscount in LWO file %s", name.c_str() );
		}
		if ( tri->numVerts > layer->polygon.count * 3 ) {
			Sys_Error( "ConvertLWOToModelSurfaces: vertex miscount in LWO file %s", name.c_str() );
		}

		// now allocate and generate the combined vertexes
		R_AllocStaticTriSurfVerts( tri, tri->numVerts );
		for ( j = 0; j < tri->numVerts; j++ ) {
			mv = &mvTable[j];
			tri->verts[ j ].Clear();
			tri->verts[ j ].xyz = vList[ mv->v ];
			tri->verts[ j ].st = tvList[ mv->tv ];
			tri->verts[ j ].normal = mv->normal;
			*(unsigned *)tri->verts[j].color = *(unsigned *)mv->color;
		}

		R_StaticFree( mvTable );
		R_StaticFree( mvHash );
	}

	R_StaticFree( tvRemap );
	R_StaticFree( vRemap );
	R_StaticFree( tvList );
	R_StaticFree( vList );

	return true;
}

srfTriangles_t* Mesh::GetGeometries( int idx )
{
	return _geometries[idx];
}

int Mesh::GetGeometriesCount()
{
	return 0;
}

void Mesh::SetJoint(Joint* root)
{
	_root = root;
}

Joint* Mesh::GetRootJoint()
{
	return _root;
}

void Mesh::SetNumFrames(int num)
{
	_numFrames = num;
}