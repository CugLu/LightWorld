#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "glutils.h"
#include "common/Array.h"

class Image {
public:

    Image();
    virtual ~Image();

    int GetWidth() const { return _width; }
    int GetHeight() const { return _height; }
    int GetDepth() const { return _depth; }
    int GetMipLevels() const { return _levelCount; }
    int GetFaces() const { return _faces; }
    GLenum GetFormat() const { return _format; }
    GLenum GetInternalFormat() const { return _internalFormat; }
    GLenum GetType() const { return _type; }
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
