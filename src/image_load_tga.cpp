#include "image_load.h"
#include "common/File.h"
#include "Image.h"

// these structs are also used in the TGA writer
// ×Ö½Ú¶ÔÆä
typedef struct _TargaHeader {
	unsigned char 	id_length, colormap_type, image_type;
	unsigned short	colormap_index, colormap_length;
	unsigned char	colormap_size;
	unsigned short	x_origin, y_origin, width, height;
	unsigned char	pixel_size, attributes;
} TargaHeader;

/*
=============
LoadTGA
=============
*/
bool image_load_tga( const char *filename, Image* i ) 
{
	lfFile* file = new lfFile;
	if ( !file->Open(filename) )
		return false;


	int	columns, rows, numPixels;
	TargaHeader targa_header;

	targa_header.id_length = file->ReadUnsignedChar();
	targa_header.colormap_type = file->ReadUnsignedChar();
	targa_header.image_type = file->ReadUnsignedChar();
	targa_header.colormap_index = file->ReadUnsignedShort();
	targa_header.colormap_length = file->ReadUnsignedShort();
	targa_header.colormap_size = file->ReadUnsignedChar();
	targa_header.x_origin = file->ReadUnsignedShort();
	targa_header.y_origin = file->ReadUnsignedShort();
	targa_header.width = file->ReadUnsignedShort();
	targa_header.height = file->ReadUnsignedShort();
	targa_header.pixel_size = file->ReadUnsignedChar();
	targa_header.attributes = file->ReadUnsignedChar();

	columns = targa_header.width;
	rows = targa_header.height;
	numPixels = columns * rows;

	if ( targa_header.id_length != 0 ) {
		file->Skip(targa_header.id_length);
 // skip TARGA image comment
	}
//
	int pixelSize = targa_header.pixel_size;
	int dataSize = numPixels*pixelSize/8;

	unsigned char* data = new unsigned char[dataSize];

	if ( targa_header.image_type == 2 || targa_header.image_type == 3 )
	{ 
		file->Read(data, dataSize);
	}
	else if ( targa_header.image_type == 10 ) {   // Runlength encoded RGB images

	}

	switch (targa_header.pixel_size)
	{
	case 8:
		i->_format = GL_BGR;
		i->_internalFormat = GL_RGBA;
		break;
	case 24:
		i->_format = GL_RGB;
		i->_internalFormat = GL_RGBA;
		break;
	case 32:
		i->_format = GL_RGBA;
		i->_internalFormat = GL_RGBA;
		break;
	default:
		break;
	}

	i->_width = targa_header.width;
	i->_height = targa_header.height;
	i->_elementSize = targa_header.pixel_size;
	i->_data.push_back( data);
	i->_levelCount = 1;
	i->_faces = 0;
	i->_depth = 0;

	delete file;
	return true;
}

