#ifndef __SURFGEO_H__
#define __SURFGEO_H__

#include "common/Vec3.h"
#include "common/Aabb3d.h"
#include "DrawVert.h"

#include "glutils.h"

class SurfTri
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
};


class DrawSurf
{
	int	id;
	material_t* shaderParms;
	SurfTri* geo;
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
};

#endif


