#include "PipelineP.h"
#include "r_public.h"
#include "renderer/draw_common.h"
#include "Shader.h"

static void draw_coordings()
{
	float vertices[] = {0.f, 0.f, 0.f,

		0.f, 10.f, 0.f,
		0.f, 0.f, 10.f,
		10.f, 0.f, 0.f
	};

	float color[] = {1.f, 0.f, 0.f, 1.f,
		1.f, 0.f, 0.f, 1.f,
		0.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
	};

	unsigned short indices[] = {0, 1, 0, 2, 0, 3};
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, color);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_SHORT, indices);
};

PipelineP::PipelineP() :mProgram(NULL)
{
}

PipelineP::~PipelineP()
{
}

void PipelineP::Render( drawSurf_t* drawSurf )
{
	mat4 t = (*drawSurf->viewProj) * drawSurf->matModel;
	glUniformMatrix4fv(mWvp, 1, GL_FALSE, &t.m[0] );
	srfTriangles_t* tris = drawSurf->geo;

	//R_DrawPositonColor(tris);
	draw_coordings();
	
	GL_CheckError("render");
}

void PipelineP::Init()
{
	Shader* shader = new Shader;
	shader->LoadAndCompile("../../../media/shader/positioncolor.vert",
		"../../../media/shader/positioncolor.frag");
	shader->PreLink();
	shader->BindAttribLocation(eAttrib_Position);
	shader->BindAttribLocation(eAttrib_Color);
	shader->Link();

	mProgram = shader->GetProgarm();

	mWvp = glGetUniformLocation(mProgram, "WVP");
}

void PipelineP::Begin()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(5);
	glUseProgram(mProgram);
}

void PipelineP::End()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(5);
}

void PipelineP::TestrRender()
{

	draw_coordings();
}
