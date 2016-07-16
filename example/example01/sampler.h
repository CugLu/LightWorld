#ifndef __SAMPLER_H__
#define __SAMPLER_H__

#include "Game.h"
#include "common/Vec3.h"

class Plane;
class Model;
class ResourceSystem;

class ShadowSampler : public Game
{
public:
	ShadowSampler( void );
	~ShadowSampler();

	void Init();

	void Frame();

	void ProcessEvent(sysEvent_s* event);

	void SetupCamera();

	void CreateLight();

	void CastShadowMap();

	bool HitTest(int x, int y);

private:
	Camera* _camera;

	Sprite* _lbCamera;

	Sprite* _spLight;

	Camera* _light;

	Plane* _plane;

	Model* _model;

	Vec3 _lightPos;

	ResourceSystem* _resourceSys;
};

#endif