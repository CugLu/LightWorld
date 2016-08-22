#include "sampler.h"

#include "ScriptSystem.h"
#include "ResourceSystem.h"
#include "renderer/RenderSystem.h"

#include "r_public.h"
#include "sys/sys_public.h"

#include "Camera.h"
#include "Sprite.h"
#include "Mesh.h"
#include "Shader.h"
#include "Shape.h"
#include "Model.h"

#include "MeshLoader3DS.h"
#include "MeshLoaderLwo.h"

#include "luautils.h"
#include "Shape.h"
#include "Interaction.h"

#include "VarSystem.h"

#pragma comment(lib, "LightWorld.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

Game* game = new ShadowSampler();
Texture* _texture;
Box* box;
float angle = 0;
Interaction* inter;

static bool drawCap = true;

Var testVal;

class A
{
public:
	A();
	~A();

	void init() {
		width = 100;
	}
	void init(int i) = delete;

private:
	int width;

};

A::A()
{
}

A::~A()
{
}

ShadowSampler::ShadowSampler( void ):_camera(NULL)
{
}

ShadowSampler::~ShadowSampler()
{
}

void ShadowSampler::Init()
{

	int i = testVal.GetInt();
	Sys_Printf("Initializing RenderSystem\n");

	// init resource system
	_resourceSys = new ResourceSystem;
	_resourceSys->SetSearchDir("../../../media/");

	// init render system
	glimpParms_t pram;
	pram.width = 800;
	pram.height = 600;
	pram.displayHz = 1/60;
	pram.stereo = 1/60;
	_renderSys = new RenderSystem(&pram, _resourceSys);
	_renderSys->Init();

	// init script system
	_scriptSys = new ScriptSystem;
	_scriptSys->Init();
	
	lua_State* L = _scriptSys->GetLuaState();
	
	// init camera
	_camera = new Camera;
	_camera->Setup3DCamera(800, 600);
	_camera->Rise(30);
	_camera->Forward(-30);
	_camera->LookAt(0, 0, 0);

	// render sys
	RenderSystem** renderSys = (RenderSystem **)lua_newuserdata(L, sizeof(RenderSystem*));
	*renderSys = _renderSys;
	lua_setfield(L, LUA_GLOBALSINDEX, "renderSys");

	// camera
	Camera** camera = (Camera**)lua_newuserdata(L, sizeof(Camera*));
	*camera = _camera;
	Lua_SetMetatable(L, "Camera");
	lua_setfield(L, LUA_GLOBALSINDEX, "camera");

	_renderSys->SetMainViewProj(_camera->GetViewProj());

	CreateLight();

	//_scriptSys->RunScript("script/main.lua");
	
	//box = _renderSys->CreateBox(3, 3, 3);
	//DrawVert* vert = box->GetVert(0);
	//vert->color[0] = 1.f;
	//box->UpdateVbo();
	//box->SetViewProj(_camera->GetViewProj());
	//box->SetPosition(0, 0, 0);
	//_renderSys->AddDrawSur(box->_drawSurf);
	
	_plane = new Plane(_resourceSys, 100, 100);
	_plane->SetTexture("test.png");
	_plane->SetTextureUV(2, 2);
	_plane->SetViewProj(_camera->GetViewProj());
	_renderSys->AddDrawSur(_plane->_drawSurf);
	//
	_lbCamera = new Sprite;
	_lbCamera->SetLabel("aaaaaaaa");
	_lbCamera->SetPosition(0, 200, 0);
	_renderSys->AddSprite(_lbCamera);

	_model = new Model(_resourceSys);
	_model->SetFile("ninja.b3d");
	_model->SetViewProj(_camera->GetViewProj());
	_renderSys->AddModel(_model);

	//_model = new Model(_resourceSys);
	//_model->SetFile("ninja.b3d");
	//_model->SetPosition(0, 0, 5);
	//_model->SetViewProj(_camera->GetViewProj());
	//_renderSys->AddModel(_model);
	
	_lightPos.set(128.f, 128.f, 12.f);

	//box = new Box(_resourceSys, 3, 3, 3);
	//box->SetViewProj(_camera->GetViewProj());
	//box->SetPosition(0, 0, 3);
	//_renderSys->AddDrawSur(box->_drawSurf);

	inter = new Interaction;
	drawSurf_t* surf = _model->_drawSurf;
	R_CreateSilIndice(surf->geo);
	R_IdentifySilEdges(surf->geo);
	inter->CreateInteraction(surf->geo, _lightPos, surf->matModel);
	_renderSys->AddSurfTris(inter->shadowTris);


	{
		/*	inter = new Interaction;
		drawSurf_t* surf = box->_drawSurf;
		R_IdentifySilEdges(surf->geo);
		inter->CreateInteraction(surf->geo, Vec3(_lightPos), surf->matModel);
		_renderSys->AddSurfTris(inter->shadowTris);*/
	}
}

void ShadowSampler::Frame()
{
	sysEvent_t ev = Sys_GetEvent();
	while (ev.evType != SE_NONE )
	{
		ProcessEvent(&ev);
		ev = Sys_GetEvent();
	}

	_renderSys->FrameUpdate();
	//_scriptSys->Call("frameUpdate");

	//glCullFace(GL_BACK);
	//glUseProgram(0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	_spLight->LookAtView(_camera->GetView());
	//Vec2 screenpos = _spLight->ToScreenCoord(*_camera->GetViewProj());

	//vec3 pos = _camera->GetPosition();
	//char buff[255];
	//sprintf_s(buff, "camera position %.02f %.02f %.02f screen pos: %.02f %.02f", pos.x, pos.y, 
	//	pos.z, screenpos.x, screenpos.y);
	//_defaultSprite->SetLabel(buff);
}

void ShadowSampler::ProcessEvent(sysEvent_s* event)
{
	switch (event->evType)
	{
	case SE_KEY:
		{
			if (event->evValue2 != 1)
				break;

			switch (event->evValue)
			{
			case 'w':
				_camera->Forward(0.5f);
				break;
			case 's':
				_camera->Forward(-0.5f);
				break;
			case 'q':
				_camera->Right(0.5f);
				break;
			case 'e':
				_camera->Right(-0.5f);
				break;
			case 'a':
				_camera->Yaw(10);
				break;
			case 'd':
				_camera->Yaw(-10);
				break;
			case 'z':
				_camera->RotateByY(5);
				break;
			case 'x':
				_camera->RotateByY(-5);
				break;
			case 'c':
				_camera->Pitch(5);
				break;
			case 'v':
				_camera->Pitch(-5);
				break;
			case 'j':
				_renderSys->_debugShadowVolume = !_renderSys->_debugShadowVolume;
				break;
			case 'k':
				_renderSys->_drawShadow = !_renderSys->_drawShadow;
				break;
			case K_ENTER:
				{
					inter->shadowTris->drawBegin += 3;
					inter->shadowTris->drawCount = 6;
					Sys_Printf("draw begin %d\n", inter->shadowTris->drawBegin);
				}
				break;
			case 'b':
				{
					if (drawCap)
					{
						inter->shadowTris->drawBegin = 0;
						inter->shadowTris->drawCount = inter->numCapIndices;
					}
					else
					{
						inter->shadowTris->drawBegin = 0;
						inter->shadowTris->drawCount = inter->numSilIndices;
					}
					drawCap = !drawCap;
				}
				break;

			case 'm':
				_renderSys->showShadowVolume++;
				if (_renderSys->showShadowVolume > 2)
					_renderSys->showShadowVolume = 0;
				break;
			default:
				break;
			}

			_lbCamera->SetLabelFmt("%02f, %02f", _camera->GetPosition().x, _camera->GetPosition().z);
			//_scriptSys->CallFuncI("onKey", event->evValue);
		}
		break;
	case SE_MOUSE:
		{
			_renderSys->HitTest(event->evValue, event->evValue2);
			//HitTest(event->evValue, event->evValue2);
		}
	default:
		break;
	}
}

void ShadowSampler::SetupCamera()
{
	_camera = new Camera();
	_camera->Setup3DCamera(1366, 768);
	_camera->SetPosition(-1.26f, 1.26f, 1.8f);
	//_camera->SetPosition(-126.f, 126.f, 100.f);
}

void ShadowSampler::CreateLight()
{
	//_light = new Camera();
	//_light->Setup3DCamera(1366, 768);
	//_light->SetPosition(-1.f, 1.f, -1.f);

	_spLight = new Sprite;
	_spLight->SetLabel("light");
	//30, 40, 10
	_spLight->SetViewProj(_camera->GetViewProj());
	_spLight->SetPosition(_lightPos.x, _lightPos.y, _lightPos.z);
	_renderSys->AddSprite(_spLight);
}

