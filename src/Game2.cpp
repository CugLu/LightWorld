#include "Game.h"
#include "Mesh.h"
#include "ResourceSystem.h"
#include "renderer/RenderSystem.h"
#include "Pipeline.h"
#include "../media/KnightModel.h"
#include "r_public.h"
#include "sys/sys_public.h"
#include "Camera.h"
#include "Sprite.h"
#include "Model.h"
#include "MeshLoader3DS.h"



class GameLocal : public Game
{
public:
	GameLocal( void );
	~GameLocal();

	void Init();

	void Frame();

	void ProcessEvent(sysEvent_s* event);

	void SetupCamera();

	void LoadAllModel();

	void AddStaticModel(StaticModel* model);
private:
	Camera* _camera;

	Sprite* _defaultSprite;
};
GameLocal::GameLocal( void ):_camera(NULL)
							,_defaultSprite(NULL)
{
}

GameLocal::~GameLocal()
{

}

Game* game = new GameLocal();

void GameLocal::Init()
{
	SetupCamera();
	LoadAllModel();

	_defaultSprite = new Sprite;
	_defaultSprite->SetLabel("...");
	_defaultSprite->SetPosition(0.f, 400.f, 0.f);
	renderSys->AddSprite(_defaultSprite);
}

void GameLocal::Frame()
{
	sysEvent_t ev = Sys_GetEvent();
	while (ev.evType != SE_NONE )
	{
		ProcessEvent(&ev);
		ev = Sys_GetEvent();
	}

	vec3 pos = _camera->GetPosition();
	char buff[255];
	sprintf_s(buff, "camera position %.02f %.02f %.02f", pos.x, pos.y, pos.z);
	_defaultSprite->SetLabel(buff);
}

void GameLocal::ProcessEvent(sysEvent_s* event)
{
	switch (event->evType)
	{
	case SE_KEY:
		{
			switch (event->evValue)
			{
			case 'w':
			case 'W':
				_camera->Walk(0.5f);
				break;
			case 's':
			case 'S':
				_camera->Walk(-0.5f);
				break;
			case 'A':
			case 'a':
				_camera->Yaw(5.f);
				break;
			case 'D':
			case 'd':
				_camera->Yaw(-0.5f);
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}

void GameLocal::LoadAllModel()
{
	drawSurf_t* surf = R_GenerateFloor(128.f, 128.f);
	surf->material = R_AllocMaterail();
	surf->material->tex = resourceSys->AddTexture("../media/test.png");
	surf->view = _camera->GetViewProj();
	renderSys->AddDrawSur(surf);

/*	Sprite* sprite = new Sprite;
	sprite->SetTexture("../media/drkwood2.jpg");
	renderSys->AddSprite(sprite);
	sprite->_drawSurf->view = _camera->GetViewProj()*/;

	StaticModel* model = resourceSys->AddMesh("ninja.b3d");
	AddStaticModel(model);
}

void GameLocal::SetupCamera()
{
	_camera = new Camera();
	_camera->Setup3DCamera();
	_camera->SetPosition(0.f, 10.f, 20.f);
}

void GameLocal::AddStaticModel(StaticModel* model)
{
	array<drawSurf_t*> surfaces = model->getSurfaces();
	for (unsigned int i = 0; i < surfaces.size(); i++)
	{
		surfaces[i]->view = _camera->GetViewProj();
	}
	renderSys->AddStaticModel(model);
}

