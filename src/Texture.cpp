#include "Texture.h"
#include "Image.h"

bool Texture::Init(Image* i)
{
	if (i== nullptr)
		return false;

	glGenTextures(1, &_name);
	glBindTexture(GL_TEXTURE_2D, _name);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	_pixelsWide = i->_width;
	_pixelsHigh = i->_height;
	int w = _pixelsWide;
	int h = _pixelsHigh;
	for (int l=0; l<i->GetMipLevels(); ++l)
	{
		if (i->IsCompressed())
		{
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, l, i->_internalFormat, (GLsizei)w, (GLsizei)h, 0, 
				i->_format, i->_type, i->GetLevel(l));
			w >>= 1;
			h >>= 1;
			w = w ? w : 1;
			h = h ? h : 1;
		}
	}

	GL_CheckError("texture:init");
	return true;
}

bool Texture::Init(int w, int h, void* data)
{
	glGenTextures(1, &_name);
	glBindTexture(GL_TEXTURE_2D, _name);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	_pixelsWide = w;
	_pixelsHigh = h;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	return true;
}

GLuint Texture::GetName()
{
	return _name;
}

bool Texture::InitCubeTexture( Image* images[6] )
{
	glGenTextures(1, &_name);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _name);

    for (int i = 0; i < 6; i++)
    {
        Image* img = images[i];

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,						// level
                         img->GetInternalFormat(),	// internal format
                         img->GetWidth(),			// width
                         img->GetHeight(),			// height
                         0,						// border
                         GL_RGBA,				// format
                         GL_UNSIGNED_BYTE,			// type
                         img->GetLevel(0));       // pixel data
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return true;
}
