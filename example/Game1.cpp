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
	StaticModel* dsmodel = new StaticModel;
	LoadMesh3DS("../Media/Dino.3ds", dsmodel);
	AddStaticModel(dsmodel);

	StaticModel* model = resourceSys->AddMesh("ninja.b3d");
	AddStaticModel(model);

	drawSurf_t* drawSur = R_AllocDrawSurf();
	drawSur->geo = R_AllocStaticTriSurf();
	srfTriangles_t* tri = drawSur->geo;
	tri->numVerts = KnightModel::numVertices;
	tri->numIndexes = KnightModel::numIndices;
	drawSur->view = _camera->GetViewProj();
	R_AllocStaticTriSurfVerts(tri, tri->numVerts);

	for (int i = 0; i < KnightModel::numVertices; i++)
	{
		memcpy(&(tri->verts[i].xyz), KnightModel::vertices[i].position, sizeof(float) * 3);
		memcpy(&(tri->verts[i].uv), KnightModel::vertices[i].uv, sizeof(float) * 3);
		memcpy(&(tri->verts[i].normal), KnightModel::vertices[i].normal, sizeof(float) * 3);
	}
	tri->indexes = new glIndex_t[KnightModel::numIndices];
	memcpy(tri->indexes, KnightModel::indices, tri->numIndexes*sizeof(glIndex_t));

	R_GenerateGeometryVbo(tri);
	renderSys->AddDrawSur(drawSur);
}

void GameLocal::SetupCamera()
{
	_camera = new Camera();
	_camera->Setup3DCamera();
}

void GameLocal::AddStaticModel(StaticModel* model)
{
	array<drawSurf_t*> surfaces = model->getSurfaces();
	for (unsigned int i = 0; i < surfaces.size(); i++)
	{
		surfaces[i]->view = _camera->GetViewProj();
		surfaces[i]->matModel.makeIdentity();
	}
	renderSys->AddStaticModel(model);
}

