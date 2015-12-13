#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "glutils.h"
#include "common/Array.h"

class Image {
public:

    Image();
    virtual ~Image();

    // return the width of the image
    int GetWidth() const { return _width; }

    //return the height of the image
    int GetHeight() const { return _height; }

    //return the dpeth of the image (0 for images with no depth)
    int GetDepth() const { return _depth; }

    //return the number of mipmap levels available for the image
    int GetMipLevels() const { return _levelCount; }

    //return the number of cubemap faces available for the image (0 for non-cubemap images)
    int GetFaces() const { return _faces; }

    //return the format of the image data (GL_RGB, GL_BGR, etc)
    GLenum GetFormat() const { return _format; }

    //return the suggested internal format for the data
    GLenum GetInternalFormat() const { return _internalFormat; }

    //return the type of the image data
    GLenum GetType() const { return _type; }

    //return the Size in bytes of a level of the image 
    int GetImageSize(int level = 0) const;

    //return whether the data is a crompressed format
	bool IsCompressed() const {
		return false;
	}

    //return whether the image represents a cubemap
    bool IsCubeMap() const { return _faces > 0; }

    //return whether the image represents a volume
    bool IsVolume() const { return _depth > 0; }

    //get a pointer to level data
    const void* GetLevel( int level, GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X) const;
    void* GetLevel( int level, GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X);

    //convert a suitable image from a cubemap cross to a cubemap (returns false for unsuitable images)
    bool ConvertCrossToCubemap();


	void FlipSurface();
public:
    int _width;
    int _height;
    int _depth;
    int _levelCount;
    int _faces;
    GLenum _format;
    GLenum _internalFormat;
    GLenum _type;
    int _elementSize;

    //pointers to the levels
    Array<GLubyte*> _data;

    void FreeData();


    //
    // Static elements used to dispatch to proper sub-readers
    //
    //////////////////////////////////////////////////////////////
    struct FormatInfo {
        const char* extension;
        bool (*reader)( const char* file, Image& i);
        bool (*writer)( const char* file, Image& i);
    };

    static FormatInfo formatTable[]; 
};
#endif
