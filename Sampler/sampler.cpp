/************************************************************************/
/* blur shader & outline shader
/************************************************************************/

#include "sampler.h"
#include "Mesh.h"
#include "ResourceSystem.h"
#include "renderer/RenderSystem.h"
#include "../media/KnightModel.h"
#include "r_public.h"
#include "sys/sys_public.h"
#include "Camera.h"
#include "Sprite.h"
#include "Model.h"
#include "MeshLoader3DS.h"
#include "Shader.h"

#pragma comment(lib, "FlipEngine.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

Game* game = new ShadowSampler();

ShadowSampler::ShadowSampler( void ):_camera(NULL)
	,_defaultSprite(NULL)
{
}

ShadowSampler::~ShadowSampler()
{
}

void ShadowSampler::Init()
{
	SetupCamera();

	_defaultSprite = new Sprite;
	_defaultSprite->SetLabel("...");
	_defaultSprite->SetPosition(0.f, 540.f, 0.f);
	renderSys->AddSprite(_defaultSprite);

	Shader* shader = new Shader;
	shader->LoadFromFile("../media/shader/shadowmap.vert", "../media/shader/shadowmap.frag");
	shader->BindAttribLocation(eAttrib_Position);
	shader->GetUniformLocation(eUniform_MVP);

	CreateLight();

	srfTriangles_t* tri = R_AllocStaticTriSurf();;
	tri->numVerts = KnightModel::numVertices;
	tri->numIndexes = KnightModel::numIndices;
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

	material_t* material = R_AllocMaterail();
	material->shader = shader;

	drawSurf_t* drawSur = R_AllocDrawSurf();
	drawSur->geo = tri;
	drawSur->material = material;
	drawSur->view = _light->GetViewProj();
	drawSur->bShaowmap = true;
	renderSys->AddDrawSur(drawSur);

	drawSur = R_AllocDrawSurf();
	drawSur->geo = tri;
	drawSur->view = _camera->GetViewProj();
	renderSys->AddDrawSur(drawSur);
}

void ShadowSampler::Frame()
{
	sysEvent_t ev = Sys_GetEvent();
	while (ev.evType != SE_NONE )
	{
		ProcessEvent(&ev);
		ev = Sys_GetEvent();
	}

	vec2 screenpos = _spLight->ToScreenCoord(*_camera->GetViewProj());

	vec3 pos = _camera->GetPosition();
	char buff[255];
	sprintf_s(buff, "camera position %.02f %.02f %.02f %.02f %.02f", pos.x, pos.y, 
		pos.z, screenpos.x, screenpos.y);
	_defaultSprite->SetLabel(buff);
}

void ShadowSampler::ProcessEvent(sysEvent_s* event)
{
	switch (event->evType)
	{
	case SE_KEY:
		{
			switch (event->evValue)
			{
			case 'w':
			case 'W':
				_camera->Walk(0.1f);
				break;
			case 's':
			case 'S':
				_camera->Walk(-0.1f);
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

void ShadowSampler::SetupCamera()
{
	_camera = new Camera();
	_camera->Setup3DCamera();
	_camera->SetPosition(-1.26f, 1.26f, 1.8f);
}

void ShadowSampler::AddStaticModel(StaticModel* model)
{
	array<drawSurf_t*> surfaces = model->getSurfaces();
	for (unsigned int i = 0; i < surfaces.size(); i++)
	{
		surfaces[i]->view = _camera->GetViewProj();
	}
	renderSys->AddStaticModel(model);
}

void ShadowSampler::CreateLight()
{
	_light = new Camera();
	_light->Setup3DCamera();
	_light->SetPosition(-1.f, 1.f, -1.f);

	_spLight = new Sprite;
	_spLight->SetLabel("light");
	_spLight->SetPosition(-1.f, 1.f, -1.f);
	renderSys->AddSprite(_spLight);
}
