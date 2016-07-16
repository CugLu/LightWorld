#include "image_load.h"
#include "Image.h"

#include <stdio.h> // required for jpeglib.h

extern "C"
{
#include "jpeglib/jpeglib.h" 
}
#include <setjmp.h>

struct loader_jpeg_error_mgr
{
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

void init_source (j_decompress_ptr cinfo)
{
	// DO NOTHING
}

boolean fill_input_buffer (j_decompress_ptr cinfo)
{
	// DO NOTHING
	return 1;
}

void skip_input_data (j_decompress_ptr cinfo, long count)
{
	jpeg_source_mgr * src = cinfo->src;
	if(count > 0)
	{
		src->bytes_in_buffer -= count;
		src->next_input_byte += count;
	}
}

void term_source (j_decompress_ptr cinfo)
{
	// DO NOTHING
}

void error_exit(j_common_ptr cinfo)
{
}

void output_message(j_common_ptr cinfo)
{
}

bool image_load_jpg(const char *file, Image* i)
{
	FILE* fp = fopen(file, "rb");
	if (fp == NULL)
		return false;

	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	unsigned char* buff = new unsigned char[size];
	fseek(fp, 0, SEEK_SET);
	fread(buff, 1, size, fp);

	// allocate and initialize JPEG decompression object
	jpeg_decompress_struct cinfo;
	loader_jpeg_error_mgr jerr;
	unsigned char** rowPtr;

	//We have to set up the error handler first, in case the initialization
	//step fails.  (Unlikely, but it could happen if you are out of memory.)
	//This routine fills in the contents of struct jerr, and returns jerr's
	//address which we place into the link field in cinfo.

	cinfo.err = jpeg_std_error(&jerr.pub);
	cinfo.err->error_exit = error_exit;
	cinfo.err->output_message = output_message;

	// compatibility fudge:
	// we need to use setjmp/longjmp for error handling as gcc-linux
	// crashes when throwing within external c code
	if (setjmp(jerr.setjmp_buffer))
	{
		// If we get here, the JPEG code has signaled an error.
		// We need to clean up the JPEG object and return.

		jpeg_destroy_decompress(&cinfo);

		delete [] buff;
		// if the row pointer was created, we delete it.
		if (rowPtr)
			delete [] rowPtr;

		// return null pointer
		return false;
	}

	// Now we can initialize the JPEG decompression object.
	jpeg_create_decompress(&cinfo);

	// specify data source
	jpeg_source_mgr jsrc;

	// Set up data pointer
	jsrc.bytes_in_buffer = size;
	jsrc.next_input_byte = (JOCTET*)buff;
	cinfo.src = &jsrc;

	jsrc.init_source = init_source;
	jsrc.fill_input_buffer = fill_input_buffer;
	jsrc.skip_input_data = skip_input_data;
	jsrc.resync_to_restart = jpeg_resync_to_restart;
	jsrc.term_source = term_source;

	jpeg_read_header(&cinfo, TRUE);

	bool useCMYK=false;
	if (cinfo.jpeg_color_space==JCS_CMYK)
	{
		cinfo.out_color_space=JCS_CMYK;
		cinfo.out_color_components=4;
		useCMYK=true;
	}
	else
	{
		cinfo.out_color_space=JCS_RGB;
		cinfo.out_color_components=3;
		i->_internalFormat = GL_RGB;
		i->_elementSize = 3;
		i->_format = GL_RGB;
	}
	cinfo.output_gamma=2.2;
	cinfo.do_fancy_upsampling=FALSE;

	jpeg_start_decompress(&cinfo);

	unsigned short rowspan = cinfo.image_width * cinfo.out_color_components;
	unsigned int width = cinfo.image_width;
	unsigned int height = cinfo.image_height;

	unsigned char* output = new unsigned char[rowspan * height];

	// Here we use the library's state variable cinfo.output_scanline as the
	// loop counter, so that we don't have to keep track ourselves.
	// Create array of row pointers for lib
	rowPtr = new unsigned char* [height];

	for( unsigned int ii = 0; ii < height; ii++ )
		rowPtr[ii] = &output[ ii * rowspan ];

	unsigned int rowsRead = 0;

	while( cinfo.output_scanline < cinfo.output_height )
		rowsRead += jpeg_read_scanlines( &cinfo, &rowPtr[rowsRead], cinfo.output_height - rowsRead );

	delete [] rowPtr;
	// Finish decompression

	jpeg_finish_decompress(&cinfo);

	// Release JPEG decompression object
	// This is an important step since it will release a good deal of memory.
	jpeg_destroy_decompress(&cinfo);

	i->_width = width;
	i->_height = height;
	unsigned char* imageData;
	// convert image
	if (useCMYK)
	{
		imageData = new unsigned char[3 * width*height];
		if (imageData)
		{
			for (int ii = 0, j = 0; ii < size; ii += 3, j += 4)
			{
				// Also works without K, but has more contrast with K multiplied in
				//				data[i+0] = output[j+2];
				//				data[i+1] = output[j+1];
				//				data[i+2] = output[j+0];
				imageData[ii + 0] = (char)(output[j + 2] * (output[j + 3] / 255.f));
				imageData[ii + 1] = (char)(output[j + 1] * (output[j + 3] / 255.f));
				imageData[ii + 2] = (char)(output[j + 0] * (output[j + 3] / 255.f));
			}
		}
		delete[] output;
	}
	else
		imageData = output;

	i->_data.push_back(imageData);
    i->_levelCount = 1;
    i->_faces = 0;
    i->_depth = 0;
	i->FlipSurface();
	return true;
}