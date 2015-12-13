#ifndef __RENDERSYSTEM_H__
#define	__RENDERSYSTEM_H__ 
#include "glutils.h"
#include "common/Mat4.h"
#include "common/Array.h"
#include "r_public.h"

class Pipeline;
class Model;
class Sprite;
class Material;
class Camera;
class AniModel;
class Box;
class Plane;
class ResourceSystem;

class RenderSystem
{
public:
	virtual ~RenderSystem() {}

	virtual void Init() = 0;

	virtual void FrameUpdate() = 0;

	virtual void DrawString(const char* text) = 0;

	virtual bool AddDrawSur(drawSurf_t* drawSur) = 0;

	virtual bool AddSprite(Sprite* sprite) = 0;

	virtual bool AddModel(Model* model) = 0;

	virtual bool AddUISurf(drawSurf_t* drawSurf) = 0;

	virtual bool AddAnimModel(AniModel* model) = 0;

	virtual Sprite* CreateSprite() = 0;

	virtual Model* CreateModel() = 0;

	virtual AniModel* CreateAniModel() = 0;

	virtual Box* CreateBox() = 0;

	virtual Plane* CreatePlane() = 0;

	virtual int GetNumSurf() = 0;

	virtual void SetMainViewProj(mat4* mat) = 0;
};

class RenderSystemLocal : public RenderSystem
{
public:
	RenderSystemLocal(glimpParms_t *glimpParms_t);
	~RenderSystemLocal() {}

	void Init();
	void FrameUpdate();
	void DrawString(const char* text);

	void RenderShadowMap(drawSurf_t* drawSur);

	virtual bool AddDrawSur(drawSurf_t* drawSur);

	virtual bool AddSprite(Sprite* sprite);

	virtual bool AddModel(Model* model);

	virtual bool AddUISurf(drawSurf_t* drawSurf);

	virtual bool AddAnimModel(AniModel* model);

	virtual int GetNumSurf(){ return _surfaces.size(); }

	virtual Sprite* CreateSprite();

	virtual Model* CreateModel();

	virtual AniModel* CreateAniModel();

	virtual Box* CreateBox();
	
	virtual Plane* CreatePlane();

	virtual void SetMainViewProj(mat4* mat) {_mainViewProj = mat;}
private:
	
	void RenderCommon();

	void RenderPasses();

	void RenderBounds();

	mat4* _mainViewProj;
private:
	Camera* _camera;
	Array<drawSurf_t*> _surfaces;
	Sprite*	_defaultSprite;
	shadowMap_t* _shadowMap;

	int _winWidth;
	int _winHeight;
	
	ResourceSystem* _resourceSys;
};

#endif
