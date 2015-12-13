#ifndef __GLUTILS_H__
#define __GLUTILS_H__

#ifdef _WIN32
//#  include <GL/glew.h>
#include "gl/glew.h"
#elif __APPLE__
#  include <openGLES/ES2/gl.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif


#include <stdio.h>
#include <stdlib.h>

class Texture;
typedef unsigned short glIndex_t;

typedef struct {
	int			width;
	int			height;
	bool		fullScreen;
	bool		stereo;
	int			displayHz;
	int			multiSamples;
} glimpParms_t;

GLuint GL_GenTextureRGB(int w, int h, void* data);

GLuint GL_CreateProgram(const char* pVertexSource, const char* pFragmentSource);
GLuint GL_CreateProgramFromFile(const char* vert, const char* frag);

void GL_CheckError(const char* op);

#ifdef _WIN32
void RB_SetGL2D( void );
GLuint GL_GenTextureRGBA(int w, int h, void* data);
void Test_2DDraw();
#endif

bool GL_CreateDevice(int width, int height);

void GL_SwapBuffers( void );


#endif
