#ifndef __RENDERTEXTURE_H__
#define __RENDERTEXTURE_H__

#include "Texture.h"

class RenderTexture : public Texture
{
public:
	RenderTexture(int w, int h);
	~RenderTexture();

private:
	GLuint _fbo;
};


#endif


