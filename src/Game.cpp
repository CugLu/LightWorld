/************************************************************************/
/* blur shader & outline shader
/************************************************************************/

#include "Game.h"
//#include "Mesh.h"
//#include "ResourceSystem.h"
#include "renderer/RenderSystem.h"
//#include "Pipeline.h"
//#include "../media/KnightModel.h"
//#include "r_public.h"
//#include "sys/sys_public.h"
//#include "Camera.h"
//#include "Sprite.h"
//#include "Mesh.h"
//#include "MeshLoader3DS.h"
//
//
//
//class GameLocal : public Game
//{
//public:
//	GameLocal( void );
//	~GameLocal();
//
//	void Init();
//
//	void Frame();
//
//	void ProcessEvent(sysEvent_s* event);
//
//	void SetupCamera();
//
//	void LoadBlurExample();
//
//	void LoadOutlineExample();
//
//	void AddStaticModel(StaticModel* model);
//
//private:
//	Camera* _camera;
//
//	Sprite* _defaultSprite;
//};
//
//
//
//GameLocal::GameLocal( void ):_camera(NULL)
//							,_defaultSprite(NULL)
//{
//}
//
//GameLocal::~GameLocal()
//{
//
//}
//
//Game* game = new GameLocal();
//
//void GameLocal::Init()
//{
//	SetupCamera();
//
//	LoadBlurExample();
//	LoadOutlineExample();
//
//	Sprite* sprite = new Sprite();
//	sprite->SetTexture("../media/img2_2.png");
//	renderSys->AddSprite(sprite);
//
//	_defaultSprite = new Sprite;
//	_defaultSprite->SetLabel("...");
//	_defaultSprite->SetPosition(0.f, 400.f, 0.f);
//	renderSys->AddSprite(_defaultSprite);
//}
//
//void GameLocal::Frame()
//{
//	sysEvent_t ev = Sys_GetEvent();
//	while (ev.evType != SE_NONE )
//	{
//		ProcessEvent(&ev);
//		ev = Sys_GetEvent();
//	}
//
//	vec3 pos = _camera->GetPosition();
//	char buff[255];
//	sprintf_s(buff, "camera position %.02f %.02f %.02f", pos.x, pos.y, pos.z);
//	_defaultSprite->SetLabel(buff);
//}
//
//void GameLocal::ProcessEvent(sysEvent_s* event)
//{
//	switch (event->evType)
//	{
//	case SE_KEY:
//		{
//			switch (event->evValue)
//			{
//			case 'w':
//			case 'W':
//				_camera->Walk(0.5f);
//				break;
//			case 's':
//			case 'S':
//				_camera->Walk(-0.5f);
//				break;
//			case 'A':
//			case 'a':
//				_camera->Yaw(5.f);
//				break;
//			case 'D':
//			case 'd':
//				_camera->Yaw(-0.5f);
//				break;
//			default:
//				break;
//			}
//		}
//		break;
//	default:
//		break;
//	}
//}
//
//void GameLocal::SetupCamera()
//{
//	_camera = new Camera();
//	_camera->Setup3DCamera();
//	_camera->SetPosition(0.f, 10.f, 20.f);
//}
//
//void GameLocal::AddStaticModel(StaticModel* model)
//{
//	array<drawSurf_t*> surfaces = model->getSurfaces();
//	for (unsigned int i = 0; i < surfaces.size(); i++)
//	{
//		surfaces[i]->view = _camera->GetViewProj();
//	}
//	renderSys->AddStaticModel(model);
//}
//
//void GameLocal::LoadBlurExample()
//{
//	Shader* shader = resourceSys->AddShaderFromFile("../media/blur.vs", "../media/blur.fs");
//	shader->BindAttribLocation(eAttrib_Position);
//	shader->BindAttribLocation(eAttrib_TexCoord);
//	shader->GetUniformLocation(eUniform_MVP);
//	shader->GetUniformLocation(eUniform_Samper0);
//
//	Sprite* sprite = new Sprite;
//	sprite->SetTexture("../media/img2_2.png");
//	renderSys->AddSprite(sprite);
//	material_t* mater = sprite->_drawSurf->material;
//	mater->shader = shader;
//	sprite->_drawSurf->matModel.buildTranslate(153.f, 0.f, 0.f);
//}
//
//void GameLocal::LoadOutlineExample()
//{
//	Shader* shader = resourceSys->AddShaderFromFile("../media/outline.vs", "../media/outline.fs");
//	shader->BindAttribLocation(eAttrib_Position);
//	shader->BindAttribLocation(eAttrib_TexCoord);
//	shader->GetUniformLocation(eUniform_MVP);
//	shader->GetUniformLocation(eUniform_Samper0);
//
//	Sprite* sprite = new Sprite;
//	sprite->SetTexture("../media/img2_2.png");
//	renderSys->AddSprite(sprite);
//	material_t* mater = sprite->_drawSurf->material;
//	mater->shader = shader;
//	sprite->_drawSurf->matModel.buildTranslate(286.f, 0.f, 0.f);
//}

void Game::Init()
{
}
