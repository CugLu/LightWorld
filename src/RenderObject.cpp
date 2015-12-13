#include "RenderObject.h"
#include "Texture.h"
#include "DrawVert.h"
#include "glutils.h"
#include "sys/sys_public.h"
#include "ResourceSystem.h"

RenderObject::RenderObject()
{
}

RenderObject::~RenderObject()
{

}

void RenderObject::Init()
{
	_drawSurf = R_AllocDrawSurf();

	float w = 1;
	float h = 1;

	_drawSurf->geo = R_AllocStaticTriSurf();
	R_GenerateQuad(_drawSurf->geo);
	_drawSurf->geo->verts[0].xyz = vec3(0.f, h, 0.f);
	_drawSurf->geo->verts[1].xyz = vec3(0.f, 0.f, 0.f);
	_drawSurf->geo->verts[2].xyz = vec3(w, h, 0.f);
	_drawSurf->geo->verts[3].xyz = vec3(w, 0.f, 0.f);

	_drawSurf->shaderParms = R_AllocMaterail();

	SetupVBO();
}

void RenderObject::SetTexture( const char* imgPath )
{
	_drawSurf->shaderParms->tex = resourceSys->AddTexture(imgPath);
	UpdateVertex();
}

void RenderObject::SetLabel( const char* label )
{
	if (_drawSurf->shaderParms->tex != NULL )
	{
		GLuint tex = _drawSurf->shaderParms->tex->GetName();
		glDeleteTextures(1, &tex );
	}
	_drawSurf->shaderParms->tex = resourceSys->AddText(label);
	UpdateVertex();
}

void RenderObject::UpdateVertex()
{
	//1 3
	//0 2
	Texture* texture = _drawSurf->shaderParms->tex;
	float w = (float)texture->_pixelsWide;
	float h = (float)texture->_pixelsHigh;

	srfTriangles_t* tri = _drawSurf->geo;
	tri->verts[0].xyz.y = h;
	tri->verts[2].xyz = vec3(w, h, 0.f);
	tri->verts[3].xyz.x = w;

	SetupVBO();
}


void RenderObject::SetupVBO()
{
	R_GenerateGeometryVbo(_drawSurf->geo);
}

void RenderObject::SetPosition(float x, float y, float z)
{
	_position.set(x, y, z);
	_drawSurf->matModel.buildTranslate(x, y, z);
}

vec3 RenderObject::GetPosition()
{
	return _position;
}

void RenderObject::LookAtView( mat4* view )
{
	_drawSurf->matModel.buildTranslate(_position);
	_drawSurf->matModel = R_BillboardModelView(_drawSurf->matModel, *view);
}

vec2 RenderObject::ToScreenCoord( mat4& viewProj )
{
	mat4 model;
	model.buildTranslate(_position);
	vec4 out = viewProj * model * vec4(_position, 1.0f);

	out.x /= out.w;
	out.y /= out.w;
	out.z /= out.w;

	// Map x, y and z to range 0-1
	out.x = out.x * 0.5f + 0.5f;
	out.y = out.y * 0.5f + 0.5f;
	out.z = out.z * 0.5f + 0.5f;

	// Map x,y to viewport
	out.x = out.x * 800.f;
	out.y = (1-out.y) * 600.f;

	vec2 screenPos;
	screenPos.x = out.x;
	screenPos.y = out.y;

	_drawSurf->matModel.buildTranslate(screenPos.x, screenPos.y, 0.f);
	return screenPos;
}

void RenderObject::SetSprite( float w, float y )
{

}
