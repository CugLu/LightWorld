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

	void BeginPosPass(float r, float g, float b);

	void BeginPosPass1();


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

public:
	srfTriangles_t* _testtri;
};

#endif
