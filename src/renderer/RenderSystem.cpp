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

#include "Interaction.h"

#include "pipeline/PipelineP.h"

drawSurf_t* draw;
void R_RenderSurfaces(Array<drawSurf_t*> surfaces)
{
	for (unsigned int i = 0; i < surfaces.size(); i++)
		R_RenderCommon(surfaces[i]);
}

RenderSystem::RenderSystem(glimpParms_t *glimpParms, ResourceSystem* resourceSys):debug(2), showShadowVolume(0)
{
	_winWidth = glimpParms->width;
	_winHeight = glimpParms->height;
	_resourceSys = resourceSys;
}

void RenderSystem::Init()
{	
	//glShadeModel(GL_SMOOTH);				
	glClearColor(0.0f, 0.5f, 0.5f, 0.5f);	

	glClearDepthf(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);								
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	glViewport(0, 0, _winWidth, _winHeight);

	// 文本需要
	glEnable(GL_BLEND);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	//glActiveTexture(GL_TEXTURE1);

	// _renderbuffer init
	_camera = new Camera;
	_camera->Setup2DCamera(_winWidth, _winHeight);
	
	// fps  init
	_defaultSprite = new Sprite;
	_defaultSprite->SetLabel("aaa");
	AddSprite(_defaultSprite);
	GL_CheckError("frameupdate");

	_pipeline = new PipelineP();
	_pipeline->Init();
	draw = new drawSurf_t;
}

void RenderSystem::FrameUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glViewport(0, 0, 800, 600);

	switch (debug)
	{
	case 1:
		RenderCommon();
		break;
	case 2:
		{
			if (showShadowVolume > 0)
			{
				glPolygonMode(GL_FRONT, GL_FILL);
				glCullFace(GL_BACK);
				glEnable(GL_BLEND);
				RenderCommon();

				//glPolygonMode(GL_FRONT, GL_LINE);
				glDisable(GL_BLEND);
				glEnableVertexAttribArray(0);
				Material* mtr = _resourceSys->FindMaterial("mtr/position.mtr");
				Shader* shader = &mtr->_shader;
				glUseProgram(shader->GetProgarm());

				mat4 t = *_mainViewProj;
				glUniformMatrix4fv( shader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0] );

				if (showShadowVolume == 1)
				{
					glCullFace(GL_BACK);
					glPolygonMode(GL_FRONT, GL_LINE);
				}
				else
				{
					glCullFace(GL_FRONT);
					glPolygonMode(GL_BACK, GL_LINE);
				}

				glDisable(GL_STENCIL_TEST);
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				R_DrawPositon(_srftri[0]);
			}
			else
			{
				glCullFace(GL_BACK);
				RB_STD_FillDepthBuffer();
				RB_DrawInteractions();
			}
	
		}
		break;
	case 3:

		break;
	case 4: 
		{
			glPolygonMode(GL_FRONT, GL_LINE);
			RenderCommon();
		}
		break;
	case 5:
		{
			_pipeline->Begin();
/*			for (unsigned int i=0; i<_surfaces.size(); ++i)
			{
				pipeline->Render(_surfaces[i]);
			}*/

			glPolygonMode(GL_FRONT, GL_LINE);
			draw->viewProj = _mainViewProj;
			for (unsigned int i=0; i<_srftri.size(); ++i)
			{
				draw->geo = _srftri[i];
				_pipeline->Render(draw);
			}
			_pipeline->TestrRender();
			_pipeline->End();
		}
		break;
	default:
		break;
	}

	GL_SwapBuffers();
}

void RenderSystem::DrawString( const char* text )
{
	_defaultSprite->SetLabel(text);
}

bool RenderSystem::AddDrawSur( drawSurf_t* drawSur )
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
	
	Sys_Printf("user draw surfce size %d\n", _surfaces.size() - 1);
	return true;
}

void RenderSystem::RenderCommon()
{
	for (unsigned int i = 0; i < _surfaces.size(); i++)
	{
		R_RenderCommon(_surfaces[i]);
		GL_CheckError("i=====");
	}
}

bool RenderSystem::AddUISurf( drawSurf_t* drawSurf )
{
	if (drawSurf->viewProj == NULL)
		drawSurf->viewProj = _camera->GetViewProj();

	if (drawSurf->tex == NULL)
		drawSurf->tex = _resourceSys->FindTexture(".png");

	drawSurf->mtr = _resourceSys->FindMaterial("mtr/positiontex.mtr");
	_sprites.push_back(drawSurf);
	return true;
	//return AddDrawSur(drawSurf);
}

bool RenderSystem::AddSprite( Sprite* sprite )
{
	drawSurf_t* drawSurf = sprite->_drawSurf;
	AddUISurf(drawSurf);
	return true;
}

bool RenderSystem::AddModel( Model* model )
{
	drawSurf_t* drawSurf = model->_drawSurf;
	if (!drawSurf->mtr)
		drawSurf->mtr = _resourceSys->FindMaterial("mtr/positiontex.mtr");
	AddDrawSur(drawSurf);

	_models.push_back(model);

	return true;
}

bool RenderSystem::AddAnimModel( AniModel* model )
{
	drawSurf_t* drawSurf = model->_drawSurf;
	drawSurf->mtr = _resourceSys->FindMaterial("mtr/positiontex.mtr");
	AddDrawSur(drawSurf);
	return true;
}

void RenderSystem::CreateShadow()
{
	srfTriangles_t* tri;
	Vec3 light = Vec3(128, 128, 12);
	mat4 matModel;
	for (unsigned int i=0; i<_models.size(); ++i)
	{
		tri = _models[i]->_drawSurf->geo;
		matModel = _models[i]->_drawSurf->matModel;
		_models[i]->_inter->CreateInteraction(tri, light, matModel);
	}
}

void RenderSystem::AddSurfTris( srfTriangles_t* tri )
{
	_srftri.push_back(tri);
}

void RenderSystem::RB_STD_FillDepthBuffer()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(5);
	Material* mtr = _resourceSys->FindMaterial("mtr/positionc.mtr");
	Shader* shader = &mtr->_shader;
	glUseProgram(shader->GetProgarm());

	mat4 t = *_mainViewProj;
	glUniformMatrix4fv( shader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0] );

	glBlendFunc( GL_ONE, GL_ZERO );
	for (unsigned int i = 0; i < _surfaces.size(); i++)
	{
		drawSurf_t* drawSurf = _surfaces[i];
		srfTriangles_t* tris = drawSurf->geo;

		mat4 t = (*drawSurf->viewProj) * drawSurf->matModel;
		glUniformMatrix4fv(shader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0] );

		R_DrawPositonColor(tris);

		GL_CheckError(__FUNCTION__);
	}
}

void RenderSystem::RB_DrawInteractions()
{
	glEnable(GL_STENCIL_TEST);
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	glClearStencil(128);
	glDepthMask( GL_FALSE );
	glStencilFunc(GL_ALWAYS, 0, 0xff);
	
	//glPolygonMode(GL_FRONT, GL_LINE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	R_RenderVolume(_srftri);

	glPolygonMode(GL_FRONT, GL_FILL);
	glCullFace(GL_BACK);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 128, 255);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glDepthMask(GL_FALSE);

	glDepthFunc(GL_LEQUAL);
	glBlendFunc( GL_ONE, GL_ONE );
	RenderCommon();

	glDepthMask(GL_TRUE);
	glDisable(GL_STENCIL_TEST);
	glPolygonMode(GL_FRONT, GL_FILL);
}

void RenderSystem::BeginPosPass()
{
	Material* mtr = _resourceSys->FindMaterial("mtr/positionc.mtr");
	if (mtr == NULL)
		return;

	unsigned short* attri = mtr->_attriArr;
	unsigned short numAttri = mtr->_numAttri;
	Shader* shader = &mtr->_shader;

	glUseProgram(shader->GetProgarm());

	for (int i = 0; i < numAttri; i++)
		glEnableVertexAttribArray(attri[i]);

	mat4 t = *_mainViewProj;
	glUniformMatrix4fv(shader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0] );

	for (unsigned int i=0; i<_srftri.size(); ++i)
		R_DrawPositonColor(_srftri[i]);

	GL_CheckError("renderpos");
}

void RenderSystem::HitTest( int x, int y )
{

}

