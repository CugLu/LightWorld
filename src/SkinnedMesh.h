#pragma once
#include "vec3.h"
#include <vector>

using std::vector;

class Mesh;
class Joint;

class SkinnedMesh 
{
public:
	SkinnedMesh() {}
	SkinnedMesh(Mesh* mesh, Joint* root, float totalFrame);
	~SkinnedMesh();
	
	void traverse(Joint* pJoint);
	void init();
	void init(Mesh* mesh, Joint* root, float totalFrame);
	void update(float fElapsedTime);
	void updateMesh(); 
	void updateVertex(Joint* joint, float frame);

	vec3* getSkinnedmeshPositions();

protected:
	Joint*			_rootJoint;
	vector<Joint*>  _allJoint;
	Mesh*			_mesh;

	// animation positions
	vec3*   _skinnedMeshPositions;

	float	_startFrame;
	float	_endFrame;
	float	_currentFrame;
	float   _totalFrame;
};