#ifndef __RENDERBUFFER_H__
#define __RENDERBUFFER_H__
#include "glutils.h"

#include "common/Mat4.h"
#include "DrawVert.h"
#include "common/Aabb3d.h"
#include "common/Joint.h"

class DrawVert;
class Shader;
class Material;

// our only drawing geometry type
typedef struct srfTriangles_s 
{
	Vec3* basePoses;

	int	numVerts;				
	DrawVert* verts;				

	int	numIndexes;			
	glIndex_t* indexes;

	GLuint vbo[2];

	Aabb3d aabb;

	bool generateNormals;
	bool tangentsCalculated;
}srfTriangles_t;

typedef struct
{
	Shader* shader;
	Texture* tex;
	Texture* bumpMap;
	Vec3 color;
}material_t;


typedef struct{
	int	id;
	material_t* shaderParms;
	srfTriangles_t*	geo;
	Material* mtr;
	mat4* view;
	mat4* proj;
	mat4* viewProj;
	mat4 matModel;

	Vec3 eyePos;
	Vec3 lightPos;

	bool bShaowmap;
	bool bShowBound;
	bool bHit;
} drawSurf_t;

typedef struct
{
	GLuint fbo;
	GLuint rbo;
	GLuint texId;
	unsigned int width;
	unsigned int height;
}shadowMap_t;

drawSurf_t* R_AllocDrawSurf();

srfTriangles_t *R_AllocStaticTriSurf( void );

void	R_AllocStaticTriSurfVerts( srfTriangles_t *tri, int numVerts );

void	R_AllocStaticTriSurfIndexes( srfTriangles_t *tri, int numIndexes );

material_t* R_AllocMaterail();


/**
 * fast methods
 **/
drawSurf_t* R_GenerateFloor(float w, float h);

void R_GenerateGeometryVbo( srfTriangles_t *tri);

void R_GenerateQuad(srfTriangles_t* geo);

shadowMap_t* R_GenerateShadowMap();

drawSurf_t* R_GenerateQuadSurf();

void R_GenerateBox( srfTriangles_t* geo, float sx, float sy, float sz);

void R_GeneratePlane(srfTriangles_t* geo, float w, float h);

mat4 R_BillboardModelView(mat4& model, mat4& view);

void R_DeriveNormals( srfTriangles_t *tri );

void R_BoundTriSurf( srfTriangles_t* tri );

// Only deals with vertexes and indexes, not silhouettes, planes, etc.
// Does NOT perform a cleanup triangles, so there may be duplicated verts in the result.
srfTriangles_t *	R_MergeSurfaceList( const srfTriangles_t **surfaces, int numSurfaces );
srfTriangles_t *	R_MergeTriangles( const srfTriangles_t *tri1, const srfTriangles_t *tri2 );

void R_StaticFree( void *data );

void R_FreeStaticTriSurf( srfTriangles_t *tri );

void *R_ClearedStaticAlloc( int bytes );

void *R_StaticAlloc( int bytes );



// animation
void R_InitBasePoses(srfTriangles_t* geo, Joint* joint);

void R_UpdateGeoPoses(srfTriangles_t* geo, Joint* joint, float frame);


//
Vec2 R_WorldToScreenPos(Vec3 pos, mat4* viewProj, int screenwidth, int screenheight);

#endif
