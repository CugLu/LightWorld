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
class Pipeline;

class RenderSystem
{
public:
	RenderSystem(glimpParms_t *glimpParms_t, ResourceSystem* resourceSys);
	~RenderSystem() {}

	void Init();
	void FrameUpdate();

	void DrawString(const char* text);

	bool AddDrawSur(drawSurf_t* drawSur);
	bool AddSprite(Sprite* sprite);
	bool AddModel(Model* model);
	bool AddUISurf(drawSurf_t* drawSurf);
	bool AddAnimModel(AniModel* model);
	void AddSurfTris(srfTriangles_t* tri);

	int GetSurfCount(){ return _surfaces.size(); }
	void SetMainViewProj(mat4* mat) {_mainViewProj = mat;}

	void HitTest(int x, int y);
private:
	void CreateShadow();
	void RenderCommon();
	void RenderBounds();
	void RB_STD_FillDepthBuffer();
	void RB_DrawInteractions();
	void BeginPosPass();

private:
	Camera* _camera;
	Sprite*	_defaultSprite;

	Array<drawSurf_t*> _surfaces;
	Array<Model*> _models;
	Array<srfTriangles_t*> _srftri;
	Array<drawSurf_t*> _sprites;

	int _winWidth;
	int _winHeight;

	ResourceSystem* _resourceSys;
	
	mat4* _mainViewProj;

public:
	srfTriangles_t* _testtri;

	Pipeline* _pipeline;

	bool _debugShadowVolume;
	bool _drawShadow;

	int debug;
	int showShadowVolume;
};

#endif
