#include "RenderSystem.h"
#include "glutils.h"
#include "Material.h"
#include "r_public.h"
#include "ResourceSystem.h"

#include "Shader.h"
#include "sys/sys_public.h"

#include "Sprite.h"
#include "Model.h"
#include "common/Timer.h"
#include "common/File.h"
#include "draw_common.h"
#include "Mesh.h"
#include "Camera.h"
#include "Shape.h"

RenderSystemLocal::RenderSystemLocal(glimpParms_t *glimpParms)
{
	_winWidth = glimpParms->width;
	_winHeight = glimpParms->height;
}

void RenderSystemLocal::Init()
{	
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);			// init value

	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);								
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	glViewport(0, 0, _winWidth, _winHeight);

	// 文本需要
	glEnable(GL_BLEND);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);// The initial value is GL_CCW.
	glCullFace(GL_BACK);// The initial value is GL_BACK.

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	//glActiveTexture(GL_TEXTURE1);


	// _renderbuffer init
	_camera = new Camera;
	_camera->Setup2DCamera(_winWidth, _winHeight);

	_resourceSys = new ResourceSystem;
	_resourceSys->SetSearchDir("../../../media/");
	_resourceSys->LoadGLResource();
	
	// fps  init
	_defaultSprite = new Sprite;
	_defaultSprite->SetLabel("aaa");
	AddSprite(_defaultSprite);
	GL_CheckError("frameupdate");
}


void RenderSystemLocal::FrameUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glViewport(0, 0, 800, 600);
	RenderCommon();

	//RenderPasses();

	RenderBounds();

	GL_SwapBuffers();
}

void RenderSystemLocal::RenderShadowMap(drawSurf_t* drawSur)
{
	glViewport(0, 0, 1024, 1024);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glBindFramebuffer(GL_FRAMEBUFFER, _shadowMap->fbo);
	glClear(GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);
	R_RenderShadowMap(drawSur, R_DrawPositon);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderSystemLocal::DrawString( const char* text )
{
	_defaultSprite->SetLabel(text);
}


bool RenderSystemLocal::AddDrawSur( drawSurf_t* drawSur )
{
	if (drawSur->geo->vbo[0] <= 0)
		R_GenerateGeometryVbo(drawSur->geo);

	if (drawSur->mtr == NULL)
	{
		Sys_Error("draw surface material is not\n");
		return false;
	}

	if (drawSur->viewProj == NULL)
	{
		Sys_Error("draw viewProj is null\n");
		return false;
	}
	
	_surfaces.push_back(drawSur);
	// system drawsurf count : 1
	Sys_Printf("user draw surfce size %d\n", _surfaces.size() - 1);
	return true;
}

void RenderSystemLocal::RenderBounds()
{	
	glEnableVertexAttribArray(0);
	Shader* shader = _resourceSys->FindShader(eShader_Position);
	glUseProgram(shader->GetProgarm());
	

	mat4 t = *_mainViewProj;
	glUniformMatrix4fv( shader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0] );

	float vertices[] = {0.f, 0.f, 0.f, 
			10.f, 0.f, 0.f,
			0.f, 10.f, 0.f,
			0.f, 0.f, 10.f,
	};

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);

	glUniform3f(shader->GetUniform(eUniform_Color), 1.0, 0.0, 0.0);
	unsigned short indices1[] = {0, 1};
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, indices1);

	glUniform3f(shader->GetUniform(eUniform_Color), 0.0, 1.0, 0.0);
	unsigned short indices2[] = {0, 2};
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, indices2);

	glUniform3f(shader->GetUniform(eUniform_Color), 0.0, 0.0, 1.0);
	unsigned short indices3[] = {0, 3};
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, indices3);

	for (unsigned int i = 0; i < _surfaces.size(); i++)
	{
		if(!_surfaces[i]->bShowBound)
			continue;

		if (_surfaces[i]->bHit)
			glUniform3f(shader->GetUniform(eUniform_Color), 1.0, 0.0, 0.0);
		else
			glUniform3f(shader->GetUniform(eUniform_Color), 0.0, 1.0, 0.0);

		mat4 t = (*_surfaces[i]->viewProj) * _surfaces[i]->matModel;
		glUniformMatrix4fv( shader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0] );
		RB_DrawBounds(&_surfaces[i]->geo->aabb);
	}

}

void RenderSystemLocal::RenderPasses()
{
	for (unsigned int i = 0; i < _surfaces.size(); i++)
	{
		//if (_surfaces[i]->bShaowmap)
		{
			//R_RenderPTPass(_surfaces[i], R_DrawPositonTex);
			//RenderShadowMap(_surfaces[i]);
		}
		if (_surfaces[i]->shaderParms->bumpMap != nullptr)
		{
			R_RenderBumpPass(_surfaces[i], R_DrawPositonTangent);
		}
		else if(_surfaces[i]->geo->tangentsCalculated && 0)
		{
			R_RenderPhongPass(_surfaces[i], R_DrawPositionTexNorm);
		}
		else
			R_RenderPTPass(_surfaces[i], R_DrawPositonTex);
	}
}

void RenderSystemLocal::RenderCommon()
{
	for (unsigned int i = 0; i < _surfaces.size(); i++)
	{
		if (_surfaces[i]->id == 99)
			_surfaces[i]->id = 99;

		R_RenderCommon(_surfaces[i]);
		GL_CheckError("i=====");
	}
}

bool RenderSystemLocal::AddUISurf( drawSurf_t* drawSurf )
{
	if (drawSurf->viewProj == NULL)
		drawSurf->viewProj = _camera->GetViewProj();

	drawSurf->shaderParms->shader = _resourceSys->FindShader(eShader_PositionTex);
	if (drawSurf->shaderParms->tex == NULL)
		drawSurf->shaderParms->tex = _resourceSys->AddTexture(".png");

	drawSurf->mtr = _resourceSys->AddMaterial("mtr/positiontex.mtr");
	return AddDrawSur(drawSurf);
}

bool RenderSystemLocal::AddSprite( Sprite* sprite )
{
	drawSurf_t* drawSurf = sprite->_drawSurf;
	AddUISurf(drawSurf);
	return true;
}

bool RenderSystemLocal::AddModel( Model* model )
{
	drawSurf_t* drawSurf = model->_drawSurf;
	drawSurf->shaderParms->shader = _resourceSys->FindShader(eShader_PositionTex);
	if (!drawSurf->mtr)
		drawSurf->mtr = _resourceSys->AddMaterial("mtr/positiontex.mtr");
	AddDrawSur(drawSurf);
	return true;
}

bool RenderSystemLocal::AddAnimModel( AniModel* model )
{
	drawSurf_t* drawSurf = model->_drawSurf;
	drawSurf->shaderParms->shader = _resourceSys->FindShader(eShader_PositionTex);
	drawSurf->mtr = _resourceSys->AddMaterial("mtr/positiontex.mtr");
	AddDrawSur(drawSurf);
	return true;
}

Sprite* RenderSystemLocal::CreateSprite()
{
	Sprite* sprite = new Sprite;
	sprite->_resourceSys = _resourceSys;
	return sprite;
}

Model* RenderSystemLocal::CreateModel()
{
	Model* model = new Model;
	model->_resourceSys = _resourceSys;
	return model;
}

AniModel* RenderSystemLocal::CreateAniModel()
{
	AniModel* model = new AniModel;
	model->_resourceSys = _resourceSys;
	return model;
}

Box* RenderSystemLocal::CreateBox()
{
	Box* box = new Box;
	box->_resourceSys = _resourceSys;
	box->_drawSurf->mtr = _resourceSys->AddMaterial("mtr/position.mtr");
	return box;
}

Plane* RenderSystemLocal::CreatePlane()
{
	Plane* box = new Plane;
	box->_resourceSys = _resourceSys;
	box->_drawSurf->mtr = _resourceSys->AddMaterial("mtr/position.mtr");
	return box;
}
