#include "glutils.h"
#include "common/precompiled.h"
#include "common/File.h"
#include "sys/sys_public.h"

static const int MAX_BUFFER_LEN = 1024;

static void GL_CompileError(GLuint shader, const char* source)
{
	char buf[MAX_BUFFER_LEN]; 
	GLint len;
	glGetShaderInfoLog(shader, MAX_BUFFER_LEN, &len, buf);
	Sys_Printf("Could not compile shader %s:\n %s\n", source, buf);
	glDeleteShader(shader);
}

static void GL_LinkError(GLuint program)
{
	GLint bufLength = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);

	if (bufLength) 
	{
		char* buf = (char*)malloc(bufLength);
		glGetProgramInfoLog(program, bufLength, NULL, buf);
		Sys_Printf("Could not link program:\n%s\n", buf);
		free(buf);
	}
	glDeleteProgram(program);
}

GLuint GL_CompileShader(GLenum shaderType, const char* source)
{
	GLuint shader = glCreateShader(shaderType);
	if (!shader)
		return 0;

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_TRUE)
		return shader;

	GL_CompileError(shader, source);
	return 0;
}

GLuint GL_CompileShaderFromFile(GLenum target, const char* filename)
{
	char* data = F_ReadFileData(filename);
	if (data == NULL)
		return 0;

    GLuint object = GL_CompileShader(target, data);
    delete []data;
    return object;
}

GLuint GL_LinkProgram(GLuint vert, GLuint pixel)
{
	GLuint program = glCreateProgram();
	if (program == NULL)
		return 0;

	glAttachShader(program, vert);
	glAttachShader(program, pixel);
	glLinkProgram(program);
	GLint linkStatus = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_TRUE)
		return program;

	GL_LinkError(program);
	return 0;
}

GLuint GL_LinkProgram( GLuint program, GLuint vert, GLuint frag )
{
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	GLint linkStatus = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_TRUE)
		return program;

	GL_LinkError(program);
	return 0;
}

GLuint GL_GenTextureRGB(int w, int h, void* data)
{
	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return texId;
}

GLuint GL_CreateProgram(const char* pVertexSource, const char* pFragmentSource) {
	GLuint vertexShader = GL_CompileShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader) {
		return 0;
	}

	GLuint pixelShader = GL_CompileShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader) {
		return 0;
	}

	return GL_LinkProgram(vertexShader, pixelShader);
}

GLuint GL_CreateProgram()
{
	return glCreateProgram();
}

GLuint GL_CreateProgramFromFile(const char* vert, const char* frag)
{
	GLuint v, f;

    if(! (v = GL_CompileShaderFromFile(GL_VERTEX_SHADER, vert)))
        v = GL_CompileShaderFromFile(GL_VERTEX_SHADER, &vert[3]); //skip the first three chars to deal with path differences

    if(! (f = GL_CompileShaderFromFile(GL_FRAGMENT_SHADER, frag)))
        f = GL_CompileShaderFromFile(GL_FRAGMENT_SHADER, &frag[3]); //skip the first three chars to deal with path differences

	return GL_LinkProgram(v, f);
}

#ifdef _WIN32
void RB_SetGL2D( void ) 
{
	// set 2D virtual screen size
	// glViewport( 0, 0, 800, 600);

	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	glOrtho( 0, 800, 0, 600, 0, 1 );		// always assume 640x480 virtual coordinates
	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

GLuint GL_GenTextureRGBA(int w, int h, void* data)
{
	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, w, h, 0, GL_BGRA , GL_UNSIGNED_BYTE, data);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return texId;
}

void Test_2DDraw()
{
	RB_SetGL2D();

	glColor3f (1.0, 0.0, 0.0);
	glPointSize(5);
	glBegin(GL_POINTS);
	for (int i = 0; i < 7; i++)
		glVertex2f (i*100.f, 100.f);
	glEnd();
}

#endif

void GL_CheckError(const char* op)
{
   int		err;
    char	s[64];
	int		i;

	// check for up to 10 errors pending
	for ( i = 0 ; i < 10 ; i++ ) {
		err = glGetError();
		if ( err == GL_NO_ERROR ) {
			return;
		}
		switch( err ) {
			case GL_INVALID_ENUM:
				strcpy( s, "GL_INVALID_ENUM" );
				break;
			case GL_INVALID_VALUE:
				strcpy( s, "GL_INVALID_VALUE" );
				break;
			case GL_INVALID_OPERATION:
				strcpy( s, "GL_INVALID_OPERATION" );
				break;
#ifdef _WIN32
			case GL_STACK_OVERFLOW:
				strcpy( s, "GL_STACK_OVERFLOW" );
				break;
			case GL_STACK_UNDERFLOW:
				strcpy( s, "GL_STACK_UNDERFLOW" );
				break;
#endif // _WIN32

			case GL_OUT_OF_MEMORY:
				strcpy( s, "GL_OUT_OF_MEMORY" );
				break;
			default:
				snprintf( s, sizeof(s), "%i", err);
				break;
		}

		Sys_Printf( "GL_CheckErrors: [%s] %s\n", op, s );
	}
}

