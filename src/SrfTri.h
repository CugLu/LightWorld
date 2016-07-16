#ifndef __SRFTRI_H__
#define __SRFTRI_H__

#include "common/Vec3.h"
#include "DrawVert.h"
#include "glutils.h"
#include "r_public.h"

class SrfTri 
{
	SrfTri();
	~SrfTri();

	void GenerateVbo();
	bool HitTest(Vec3 orgin, Vec3 dir);

private:
	Vec3* orginPoses;

	int	numVerts;				
	DrawVert* verts;				

	int	numIndices;			
	glIndex_t* indices;

	int numSilEdges;
	silEdge_t* silEdges;
	glIndex_t* silIndexes;
	glIndex_t* sigIndice;

	GLuint vbo[2];

	Aabb3d aabb;
	Plane* facePlanes;

	glIndex_t drawBegin;
	glIndex_t drawCount;

	bool generateNormals;
	bool tangentsCalculated;
	bool facePlanesCalculated;
	bool perfectHull;
	bool calcShadow;
	bool bDrawSingleEdge;
};

#endif