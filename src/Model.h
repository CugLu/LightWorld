#ifndef __MODEL_H__
#define __MODEL_H__
#include "common/Array.h"
#include "common/HashTable.h"
#include "r_public.h"

class Joint;
class ResourceSystem;
class Interaction;

class Model {
	friend class RenderSystem;
public:
	Model(ResourceSystem* resourceSys);
	virtual ~Model();

	virtual void Init();

	virtual void SetFile(const char* filename);

	void SetPosition(float x, float y, float z);

	Vec3 GetPosition();

	void SetViewProj(mat4* viewProj);
protected:
	Vec3 _position;

public:
	drawSurf_t* _drawSurf;
	Interaction* _inter;
	ResourceSystem* _resourceSys;
};

struct AniAction
{
	int startframe;
	int endframe;
	lfStr name;
};

class AniModel {
public:
	AniModel();
	virtual ~AniModel();

	void Init();

	void SetFile(const char* filename);	

	void Update(float elsepedtime);

	void UpdateJointPoses(Joint* joint);

	void SetPosition(float x, float y, float z);

	Vec3 GetPosition();

	void SetViewProj(mat4* viewProj);

	void PushAnimation(const char* name, int start, int end);

	void Play(const char* name, bool isLoop);
public:
	Joint* _root;
	drawSurf_t* _drawSurf;
	Vec3 _position;

	float  _startFrame;
	float  _endFrame;
	float  _currentFrame;
	float  _totalFrame;

	bool _isLoop;

	HashTable _animations;
	
	ResourceSystem* _resourceSys;
};

#endif


