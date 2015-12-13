#ifndef __MESH_H__
#define __MESH_H__
#include "r_public.h"
#include "common/Array.h"
#include "common/String.h"
#include "common/Joint.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	srfTriangles_t* AllocGeo();

	srfTriangles_t* GetGeometries(int idx);

	int GetGeometriesCount();

	void GenerateNormals();

	void CalcBounds();

	bool ConvertLWOToModelSurfaces( const struct st_lwObject *lwo );

	void SetJoint(Joint* root);

	Joint* GetRootJoint();

	void SetNumFrames(int num);

	int GetNumFrames() {return _numFrames;}

private:

	Array<srfTriangles_t*> _geometries;

	Joint* _root;

	lfStr name;

	int _numFrames;


};
#endif


