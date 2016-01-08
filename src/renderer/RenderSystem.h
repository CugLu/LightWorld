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

	virtual Box* CreateBox(int w, int h, int z) = 0;

	virtual Plane* CreatePlane(int w, int h) = 0;

	virtual int GetNumSurf() = 0;

	virtual void SetMainViewProj(mat4* mat) = 0;

	virtual void AddSurfTris(srfTriangles_t* tri) = 0;
public:
	bool _debugShadowVolume;
	bool _drawShadow;
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

	virtual Box* CreateBox(int nx, int ny, int nz);
	
	virtual Plane* CreatePlane(int w, int h);

	virtual void SetMainViewProj(mat4* mat) {_mainViewProj = mat;}

	virtual void AddSurfTris(srfTriangles_t* tri);
private:
	void CreateShadow();

	void RenderCommon();

	void RenderPasses();

	void RenderBounds();

	void RB_STD_FillDepthBuffer();

	void RB_DrawInteractions();

	mat4* _mainViewProj;
private:
	Camera* _camera;
	Sprite*	_defaultSprite;
	shadowMap_t* _shadowMap;

	Array<drawSurf_t*> _surfaces;
	Array<Model*> _models;
	Array<srfTriangles_t*> _srftri;
	Array<drawSurf_t*> _sprites;

	int _winWidth;
	int _winHeight;
	
	ResourceSystem* _resourceSys;
};

#endif
