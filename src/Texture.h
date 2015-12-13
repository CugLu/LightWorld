#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "glutils.h"

class  Image;

class Texture
{
public:
	Texture() {}
	virtual ~Texture() {}
	
	bool Init(Image* i);

	bool Init(int w, int h, void* data);

	bool InitCubeTexture(Image* images[6]);

	GLuint GetName();

    int _pixelsWide;

    int _pixelsHigh;

private:
    GLuint _name;

    /** texture max S */
    GLfloat _maxS;
    
    GLfloat _maxT;

    bool _hasPremultipliedAlpha;

    bool _hasMipmaps;

    bool _antialiasEnabled;

};


#endif