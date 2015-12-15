#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "r_public.h"
#include "ResourceSystem.h"

class Render
{
public:
	Render();
	virtual ~Render();

	void SetPosition(float x, float y, float z);

	Vec3 GetPosition();

	void SetRotation(float w, float x, float y, float z);

	void SetViewProj( mat4* viewProj );

	void UpdateTransform();
public:
	drawSurf_t* _drawSurf;

	Vec3 _position;

	Quat _rotation;

	ResourceSystem* _resourceSys;
};

class Box : public Render
{
public:
	Box(float sx = 1.f, float sy = 1.f, float sz = 1.f)
	{
		R_GenerateBox( _drawSurf->geo, sx, sy, sz);
		R_GenerateGeometryVbo(_drawSurf->geo);
	}

	~Box() {}
};


class Plane : public Render
{
public:
	Plane(int width = 1, int height = 1)
	{
		R_GeneratePlane(_drawSurf->geo, width, height);
	}

	~Plane() {}

	void SetTexture(const char* filename);

	void SetTextureUV(float u, float v);
};



#endif