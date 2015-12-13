#ifndef __SPRITE_H__
#define __SPRITE_H__
#include "r_public.h"

class Texture;
class ResourceSystem;

class RenderObject
{
public:
	RenderObject();
	~RenderObject();

	void Init();
	
	void SetModelFile(const char* filepath);

	void SetSprite(float w, float y);

	void SetTexture(const char* filepath);

	void SetLabel(const char* label);

	void SetupVBO();

	void SetPosition(float x, float y, float z);

	vec3 GetPosition();

	// billboard
	void LookAtView(mat4* view);

	vec2 ToScreenCoord(mat4& viewProj);

private:
	void UpdateVertex();

private:

public:
	drawSurf_t* _drawSurf;
	vec3 _position;
};


#endif
