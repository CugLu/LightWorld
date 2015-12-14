#include "Shape.h"

Render::Render()
{
	_drawSurf = R_AllocDrawSurf();
	_drawSurf->shaderParms = R_AllocMaterail();
	_drawSurf->geo = R_AllocStaticTriSurf();

}

Render::~Render()
{
	delete _drawSurf;
}

void Render::SetPosition(float x, float y, float z)
{
	_position.set(x, y, z);
	UpdateTransform();
}

Vec3 Render::GetPosition()
{
	return _position;
}

void Render::SetViewProj( mat4* viewProj )
{
	_drawSurf->viewProj = viewProj;
}

void Render::SetRotation( float x, float y, float z, float w )
{
	_rotation.Set(x, y, z, w);
	UpdateTransform();
}

void Render::UpdateTransform()
{
	_drawSurf->matModel = _rotation.ToMatrix();
	_drawSurf->matModel.m[12] = _position.x; 
	_drawSurf->matModel.m[13] = _position.y; 
	_drawSurf->matModel.m[14] = _position.z; 
}

void Plane::SetTexture( const char* filename )
{
	_drawSurf->shaderParms->tex = _resourceSys->AddTexture(filename);
}
