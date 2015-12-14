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
#include "Mesh.h"
#include "MeshLoader3DS.h"
#include "Shader.h"


#include "Model_lwo.h"
#include "Mesh.h"
#include "Model.h"

#pragma comment(lib, "FlipEngine.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

Game* game = new ShadowSampler();

drawSurf_t* _drawSurf;

Array<lfStr> files;

unsigned int current = 0;

Sprite* sprite;
Model* model;
Camera* camera;

ShadowSampler::ShadowSampler( void ):
	_defaultSprite(NULL)
{
}

ShadowSampler::~ShadowSampler()
{
}

void ShadowSampler::Init()
{
	//float w = 800.f;
	//float h = 600.f;
	//_drawSurf = R_GenerateQuadSurf();

	//_drawSurf->geo->verts[0].xyz = vec3(0.f, 0.f, 0.f);
	//_drawSurf->geo->verts[1].xyz = vec3(0.f, h, 0.f);
	//_drawSurf->geo->verts[2].xyz = vec3(w, 0.f, 0.f);
	//_drawSurf->geo->verts[3].xyz = vec3(w, h, 0.f);

	//_drawSurf->material = R_AllocMaterail();
	//_drawSurf->material->shader = resourceSys->FindShader(eShader_PositionTex);
	//renderSys->AddUISurf(_drawSurf);

	//animodel = render.newanimodel()
	//animodel:setFile("../Media/ninja.b3d")
	//animodel:setPosition(0, 0, -10)
	//animodel:setViewProj(camera:getViewProj())
	//render.addanimodel(animodel)
	camera = new Camera;
	camera->Init();
	camera->Setup3DCamera(800, 600);

	model = new Model;
	model->Init();
	model->SetPosition(0.f, 0.f, -20.f);
	model->SetFile("../Media/ninja.b3d");
	model->SetViewProj(camera->GetViewProj());
	_renderSys->AddModel(model);

	sprite = new Sprite;
	sprite->SetTexture("../media/engineflare1.jpg");
	sprite->SetPosition(400.f, 400.f, 0.f);
	_renderSys->AddSprite(sprite);

	Sys_ListAllFile("../media", "*", files);
	for (unsigned int i=0; i<files.size(); i++)
	{
		if (files[i].Find(".tga") != -1)
		{
			sprite->SetTexture(files[current].c_str());
			//_drawSurf->material->tex = resourceSys->AddTexture(files[i].c_str());
			break;
		}

	}
	//_drawSurf->material->tex = resourceSys->AddTexture("../media/engineflare1.jpg");


}

void ShadowSampler::Frame()
{
	sysEvent_t ev = Sys_GetEvent();
	while (ev.evType != SE_NONE )
	{
		ProcessEvent(&ev);
		ev = Sys_GetEvent();
	}


	glCullFace(GL_BACK);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
				camera->Forward(0.1f);
				break;
			case 's':
			case 'S':
				camera->Forward(-0.1f);
				break;
			case 'A':
			case 'a':
			
				break;
			case 'D':
			case 'd':
			
				break;
			case 'Q': case'q':
			
				break;;
			default:
				break;
			}
		}
		break;
	case SE_MOUSE:
		{
				ShowNext();
		}
	default:
		break;
	}
}

void ShadowSampler::ShowNext()
{
	while (true)
	{
		if (current >= files.size())
			current = 0;

		if (files[current].Find(".tga") != -1)
		{
			Sys_Printf("current texture %s\n", files[current].c_str());
			return sprite->SetTexture(files[current++].c_str());
		}
		else if(files[current].Find(".lwo") != -1)
		{
			Sys_Printf("current texture %s\n", files[current].c_str());
			return model->SetFile(files[current++].c_str());
		}

		current++;
	}

}

