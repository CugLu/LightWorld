#include "Shape.h"

Render::Render(ResourceSystem* resourceSys) : _resourceSys(resourceSys)
{
	_drawSurf = R_AllocDrawSurf();
	_drawSurf->geo = R_AllocStaticTriSurf();

}

Render::~Render()
{
	delete _drawSurf;
}

void Render::SetPosition(float x, float y, float z)
{
	_transform.position.set(x, y, z);
	UpdateTransform();
}

Vec3 Render::GetPosition()
{
	return _transform.position;
}

void Render::SetViewProj( mat4* viewProj )
{
	_drawSurf->viewProj = viewProj;
}

void Render::SetRotation( float x, float y, float z, float w )
{
	_transform.rotation.Set(x, y, z, w);
	UpdateTransform();
}

void Render::UpdateTransform()
{
	_drawSurf->matModel = _transform.rotation.ToMatrix();
	_drawSurf->matModel.m[12] = _transform.position.x; 
	_drawSurf->matModel.m[13] = _transform.position.y; 
	_drawSurf->matModel.m[14] = _transform.position.z; 
}

DrawVert* Render::GetVert( int idx )
{
	if (_drawSurf)
		return &(_drawSurf->geo->verts[idx]);

	return NULL;
}

void Render::UpdateVbo()
{
	R_GenerateGeometryVbo(_drawSurf->geo);
}

void Plane::SetTexture( const char* filename )
{
	_drawSurf->tex = _resourceSys->FindTexture(filename);
}

void Plane::SetTextureUV( float u, float v )
{
	R_SetTextureUV(_drawSurf->geo, u, v);
}
