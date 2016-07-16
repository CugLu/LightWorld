#include "image_load.h"
#include "Image.h"

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;
typedef long LONG;

struct bitmapfileheader{
	unsigned short bfType;
	unsigned long  bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long  bfOffBits;
};

struct bitmapinfoheader{
	DWORD      biSize;
	LONG       biWidth;
	LONG       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;
	LONG       biXPelsPerMeter;
	LONG       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
};

bool image_load_bmp(const char *file, Image* i)
{
	FILE *fp; 
    bitmapfileheader bitmapFileHeader; 
	bitmapinfoheader bitmapInfoHeader;
    unsigned char *bitmapImage; 
    int imageIdx = 0;   
	//our swap variable
    unsigned char tempRGB;

    //open filename in read binary mode
    fp = fopen(file, "rb");
    if (fp == NULL)
        return false;

    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(bitmapfileheader), 1, fp);

    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        fclose(fp);
        return NULL;
    }

    //read the bitmap info header
    fread(&bitmapInfoHeader, sizeof(bitmapinfoheader),1,fp); // small edit. forgot to add the closing bracket at sizeof

	bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth*bitmapInfoHeader.biHeight*(bitmapInfoHeader.biBitCount>>3);
    //move file point to the begging of bitmap data
    fseek(fp, bitmapFileHeader.bfOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader.biSizeImage);

    //verify memory allocation
    if (!bitmapImage)
    {
        free(bitmapImage);
        fclose(fp);
        return NULL;
    }

    //read in the bitmap image data
    fread(bitmapImage, bitmapInfoHeader.biSizeImage, 1, fp);

    //make sure bitmap image data was read
    if (bitmapImage == NULL)
    {
        fclose(fp);
        return NULL;
    }

    //swap the r and b values to get RGB (bitmap is BGR)
	for (unsigned int imageIdx = 0; imageIdx < bitmapInfoHeader.biSizeImage; imageIdx += 3)
    {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }

	i->_width = bitmapInfoHeader.biWidth;
	i->_height = bitmapInfoHeader.biHeight;
	i->_levelCount = 1;
	i->_format = GL_RGB;
	i->_data.push_back(bitmapImage);
    //close file and return bitmap iamge data
    fclose(fp);
    return true;
}

