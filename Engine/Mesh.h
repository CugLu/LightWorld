#ifndef __MESH_H__
#define __MESH_H__
#include "r_public.h"
#include "common/array.h"
#include "common/str.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	srfTriangles_t* AllocGeo();

	array<srfTriangles_t*>* GetGeometries();

	void GenerateNormals();

	void CalcBounds();

	bool ConvertLWOToModelSurfaces( const struct st_lwObject *lwo );

private:

	array<srfTriangles_t*> _geometries;

	lfStr name;
};
#endif


