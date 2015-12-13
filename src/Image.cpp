#include "common/precompiled.h"
#include "Image.h"

Image::Image() : _width(0), _height(0), _depth(0), _levelCount(0), _faces(0), _format(GL_RGBA),
    _internalFormat(GL_RGBA), _type(GL_UNSIGNED_BYTE), _elementSize(0) {
}

Image::~Image() {
    FreeData();
}

void Image::FreeData() {
    for (int i=0; i<_data.size(); i++) {
        delete _data[i];
    }
    _data.clear();
}

int Image::GetImageSize( int level) const {
    bool compressed = IsCompressed();
    int w = _width >> level;
    int h = _height >> level;
    int d = _depth >> level;
    w = (w) ? w : 1;
    h = (h) ? h : 1;
    d = (d) ? d : 1;
    int bw = (compressed) ? ( w + 3 ) / 4 : w;
    int bh = (compressed) ? ( h + 3 ) / 4 : h;
    int elementSize = _elementSize;

    return bw*bh*d*elementSize;
}


const void* Image::GetLevel( int level, GLenum face) const {
    assert( level < _levelCount);
    assert( _faces == 0 || ( face >= GL_TEXTURE_CUBE_MAP_POSITIVE_X && face <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z));

    face = face - GL_TEXTURE_CUBE_MAP_POSITIVE_X;

    assert( (face*_levelCount + level) < (int)_data.size());
    return _data[ face*_levelCount + level];
}


void* Image::GetLevel( int level, GLenum face) {
    assert( level < _levelCount);
    assert( _faces == 0 || ( face >= GL_TEXTURE_CUBE_MAP_POSITIVE_X && face <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z));

    face = face - GL_TEXTURE_CUBE_MAP_POSITIVE_X;

    assert( (face*_levelCount + level) < (int)_data.size());
    return _data[ face*_levelCount + level];
}

bool Image::ConvertCrossToCubemap() {
    //can't already be a cubemap
    if (IsCubeMap())
        return false;

    //mipmaps are not supported
    if (_levelCount != 1)
        return false;

    //compressed textures are not supported
    if (IsCompressed())
        return false;

    //this function only supports vertical cross format for now (3 wide by 4 high)
    if (  (_width / 3 != _height / 4) || (_width % 3 != 0) || (_height % 4 != 0) || (_depth != 0))
        return false;

    //get the source data
    GLubyte *data = _data[0];

    int fWidth = _width / 3;
    int fHeight = _height / 4;

    //remove the old pointer from the vector
    _data.erase(_data.size() - 1);
    
    GLubyte *face = new GLubyte[ fWidth * fHeight * _elementSize];
    GLubyte *ptr;

    //extract the faces

    // positive X
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        memcpy( ptr, &data[((_height - (fHeight + j + 1))*_width + 2 * fWidth) * _elementSize], fWidth*_elementSize);
        ptr += fWidth*_elementSize;
    }
    _data.push_back(face);

    // negative X
    face = new GLubyte[ fWidth * fHeight * _elementSize];
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        memcpy( ptr, &data[(_height - (fHeight + j + 1))*_width*_elementSize], fWidth*_elementSize);
        ptr += fWidth*_elementSize;
    }
    _data.push_back(face);

    // positive Y
    face = new GLubyte[ fWidth * fHeight * _elementSize];
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        memcpy( ptr, &data[((4 * fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
        ptr += fWidth*_elementSize;
    }
    _data.push_back(face);

    // negative Y
    face = new GLubyte[ fWidth * fHeight * _elementSize];
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        memcpy( ptr, &data[((2*fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
        ptr += fWidth*_elementSize;
    }
    _data.push_back(face);

    // positive Z
    face = new GLubyte[ fWidth * fHeight * _elementSize];
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        memcpy( ptr, &data[((_height - (fHeight + j + 1))*_width + fWidth) * _elementSize], fWidth*_elementSize);
        ptr += fWidth*_elementSize;
    }
    _data.push_back(face);

    // negative Z
    face = new GLubyte[ fWidth * fHeight * _elementSize];
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        for (int i=0; i<fWidth; i++) {
            memcpy( ptr, &data[(j*_width + 2 * fWidth - (i + 1))*_elementSize], _elementSize);
            ptr += _elementSize;
        }
    }
    _data.push_back(face);

    //set the new # of faces, width and height
    _faces = 6;
    _width = fWidth;
    _height = fHeight;

    //delete the old pointer
    delete []data;

    return true;
}

void Image::FlipSurface()
{
	unsigned int lineSize;

	int depth = (_depth) ? _depth : 1;

	if (!IsCompressed()) {
		lineSize = _elementSize * _width;
		unsigned int sliceSize = lineSize * _height;

		GLubyte *tempBuf = new GLubyte[lineSize];

		for (int ii = 0; ii < depth; ii++) {
			GLubyte *top = _data[0] + ii*sliceSize;
			GLubyte *bottom = top + (sliceSize - lineSize);

			for (int jj = 0; jj < (_height >> 1); jj++) {
				memcpy(tempBuf, top, lineSize);
				memcpy(top, bottom, lineSize);
				memcpy(bottom, tempBuf, lineSize);

				top += lineSize;
				bottom -= lineSize;
			}
		}

		delete[]tempBuf;
	}
}