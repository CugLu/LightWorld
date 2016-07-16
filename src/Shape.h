#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "r_public.h"
#include "ResourceSystem.h"

class Render
{
public:
	Render(ResourceSystem* resourceSys);
	virtual ~Render();

	void SetPosition(float x, float y, float z);
	Vec3 GetPosition();

	void SetRotation(float w, float x, float y, float z);
	void SetViewProj( mat4* viewProj );

	DrawVert* GetVert(int idx);
	void UpdateVbo();

	void UpdateTransform();

public:
	drawSurf_t* _drawSurf;

	transform_t _transform;

	ResourceSystem* _resourceSys;
};

class Box : public Render
{
public:
	Box(ResourceSystem* resourceSys, float sx = 1.f, float sy = 1.f, float sz = 1.f)
		: Render(resourceSys)
	{
		R_GenerateBox( _drawSurf->geo, sx, sy, sz);
		R_GenerateGeometryVbo(_drawSurf->geo);
		_drawSurf->mtr = _resourceSys->FindMaterial("mtr/position.mtr");
	}

	~Box() {}
};

class Plane : public Render
{
public:
	Plane(ResourceSystem* resourceSys, int width = 1, int height = 1)
		: Render(resourceSys)
	{
		R_GeneratePlane(_drawSurf->geo, width, height);
		_drawSurf->mtr = _resourceSys->FindMaterial("mtr/positionTex.mtr");
	}

	~Plane() {}

	void SetTexture(const char* filename);
	void SetTextureUV(float u, float v);
};

#endif