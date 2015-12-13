#ifndef __IMAGELOADER_H__
#define __IMAGELOADER_H__

class Image;

bool image_load_png(const char *file, Image* i);

bool image_load_jpg(const char *file, Image* i);

bool image_load_bmp(const char *file, Image* i);

#ifdef _WIN32
bool image_load_dds(const char *file, Image* i);
#endif // _WIN32

bool image_load_tga(const char *file, Image* i);
#endif