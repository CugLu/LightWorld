#include "Sprite.h"
#include "Texture.h"
#include "DrawVert.h"
#include "glutils.h"
#include "sys/sys_public.h"
#include "ResourceSystem.h"

Sprite::Sprite() : _width(0),
				_height(0)
{
	Init();
}

Sprite::~Sprite()
{

}

void Sprite::Init()
{
	_drawSurf = R_AllocDrawSurf();

	float w = 1;
	float h = 1;

	_drawSurf->geo = R_AllocStaticTriSurf();
	R_GenerateQuad(_drawSurf->geo);
	_drawSurf->geo->verts[0].xyz = Vec3(0.f, h, 0.f);
	_drawSurf->geo->verts[1].xyz = Vec3(0.f, 0.f, 0.f);
	_drawSurf->geo->verts[2].xyz = Vec3(w, h, 0.f);
	_drawSurf->geo->verts[3].xyz = Vec3(w, 0.f, 0.f);

	SetupVBO();
}

void Sprite::SetTexture( const char* imgPath )
{
	_drawSurf->tex = _resourceSys->FindTexture(imgPath);
	UpdateVertex();
}

void Sprite::SetLabel( const char* label )
{
	if (_drawSurf->tex != NULL )
	{
		GLuint tex = _drawSurf->tex->GetName();
		glDeleteTextures(1, &tex );
	}
	_drawSurf->tex = _resourceSys->AddText(label);
	UpdateVertex();
}

void Sprite::SetLabelFmt( const char* fmt, ... )
{
	char msg[1024];
	va_list argptr;
	va_start(argptr, fmt);
	lfStr::vsnPrintf( msg, 1023, fmt, argptr );
	va_end(argptr);
	msg[sizeof(msg)-1] = '\0';

	SetLabel(msg);
}

void Sprite::UpdateVertex()
{
	//1 3
	//0 2
	Texture* texture = _drawSurf->tex;
	float w = (float)texture->_pixelsWide;
	float h = (float)texture->_pixelsHigh;

	srfTriangles_t* tri = _drawSurf->geo;
	tri->verts[0].xyz.y = h;
	tri->verts[2].xyz = Vec3(w, h, 0.f);
	tri->verts[3].xyz.x = w;

	SetupVBO();
}

void Sprite::SetupVBO()
{
	R_GenerateGeometryVbo(_drawSurf->geo);
}

void Sprite::SetPosition(float x, float y, float z)
{
	_position.set(x, y, z);
	_drawSurf->matModel.BuildTranslate(x, y, z);
}

void Sprite::SetViewProj( mat4* viewProj )
{
	_drawSurf->viewProj = viewProj;
}

Vec3 Sprite::GetPosition()
{
	return _position;
}

void Sprite::LookAtView( mat4* view )
{
	_drawSurf->matModel.BuildTranslate(_position);
	_drawSurf->matModel = R_BillboardModelView(_drawSurf->matModel, *view);
}

Vec2 Sprite::ToScreenCoord( mat4& viewProj )
{
	Vec2 screenPos = R_WorldToScreenPos(_position, &viewProj, 800, 600);
	_drawSurf->matModel.BuildTranslate(screenPos.x, screenPos.y, 0.f);
	return screenPos;
}
