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
class Plane;
class Texture;

typedef struct {
	// NOTE: making this a glIndex is dubious, as there can be 2x the faces as verts
	glIndex_t					p1, p2;					// planes defining the edge
	glIndex_t					v1, v2;					// verts defining the edge
} silEdge_t;

// our only drawing geometry type
typedef struct srfTriangles_s 
{
	Vec3* basePoses;

	int	numVerts;				
	DrawVert* verts;				

	int	numIndices;			
	glIndex_t* indices;
	glIndex_t *	silIndices;		

	// edges
	bool bPickUp;
	int numSilEdges;
	silEdge_t* silEdges;

	// vbo
	GLuint vbo[2];

	Aabb3d aabb;

	// pick
	int numPickIndices;
	glIndex_t* pickIndices; 

	Plane* facePlanes;

	//
	glIndex_t drawBegin;
	glIndex_t drawCount;

	//
	bool generateNormals;
	bool tangentsCalculated;
	bool facePlanesCalculated;
	bool perfectHull;
	bool calcShadow;
	bool bDrawSingleEdge;

}srfTriangles_t;

typedef struct{
	int	id;
	srfTriangles_t*	geo;
	Material* mtr;
	Texture* tex;
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

typedef struct
{
	Vec3 position;
	Quat rotation;
}transform_t;

drawSurf_t* R_AllocDrawSurf();
srfTriangles_t *R_AllocStaticTriSurf( void );

void R_AllocStaticTriSurfVerts( srfTriangles_t *tri, int numVerts );
void R_AllocStaticTriSurfIndices( srfTriangles_t *tri, int numIndexes );
void R_AllocStaticTriSurfPlanes(srfTriangles_t* tri, int num);

drawSurf_t* R_GenerateFloor(float w, float h);
drawSurf_t* R_GenerateQuadSurf();
void R_GenerateGeometryVbo( srfTriangles_t *tri);
void R_GenerateQuad(srfTriangles_t* geo);
void R_GenerateBox(srfTriangles_t* geo, float sx, float sy, float sz);
void R_GeneratePlane(srfTriangles_t* geo, float w, float h);

//shadowMap_t* R_GenerateShadowMap();
void R_SetTextureUV(srfTriangles_t* geo, float u, float v);

mat4 R_BillboardModelView(mat4& model, mat4& view);

void R_DeriveNormals( srfTriangles_t *tri );

void R_BoundTriSurf( srfTriangles_t* tri );

// Only deals with vertexes and indexes, not silhouettes, planes, etc.
// Does NOT perform a cleanup triangles, so there may be duplicated verts in the result.
srfTriangles_t *	R_MergeSurfaceList( const srfTriangles_t **surfaces, int numSurfaces );
srfTriangles_t *	R_MergeTriangles( const srfTriangles_t *tri1, const srfTriangles_t *tri2 );

void R_DeriveFacePlanes( srfTriangles_t *tri );

void R_FreeStaticTriSurf( srfTriangles_t *tri );

// animation
void R_InitBasePoses(srfTriangles_t* geo, Joint* joint);

void R_UpdateGeoPoses(srfTriangles_t* geo, Joint* joint, float frame);

//
Vec2 R_WorldToScreenPos(Vec3 pos, mat4* viewProj, int screenwidth, int screenheight);

void R_IdentifySilEdges( srfTriangles_t *tri );

void R_DrawAllTris(srfTriangles_t *tri);

void R_PickTri( srfTriangles_t* tri, Vec3 orig, Vec3 dir);

void R_CreateSilIndice(srfTriangles_t* tri);

#endif
