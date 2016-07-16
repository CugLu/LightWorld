#ifndef __SHADER_H__
#define __SHADER_H__

#include "glutils.h"
#include "common/String.h"

typedef enum
{
	eUniform_MVP,
	eUniform_Color,
	eUniform_Samper0,
	eUniform_ModelView,
	eUniform_InvModelView,
	eUniform_EyePos,
	eUniform_LightPos,
	eUniform_BumpMap,

	eUniform_Count,
}unformType_t;

typedef enum
{
	eAttrib_Position,
	eAttrib_TexCoord,
	eAttrib_Normal,
	eAttrib_Tangent,
	eAttrib_Binormal,
	eAttrib_Color,

	eAttrib_Count,
}attribType_t;

typedef enum
{
	eShader_Position,
	eShader_PositionTex,
	eShader_Phong,
	eShader_Bump,
	eShader_Blur,

	eShader_Count,
}shaderType_t;



class Shader
{
public:
	Shader();
	~Shader() {}

	void	BindAttribLocation(attribType_t type);

	void	GetUniformLocation(unformType_t type);
	GLuint	GetProgarm();
	GLuint	GetUniform(unformType_t type);

	bool	LoadFromBuffer(const char* vfile, const char* ffile);
	bool	LoadFromFile(const char* vfile, const char* ffile);

	bool	SetName(const char* name);
	
	bool    LoadAndCompile(const char* vfile, const char* ffile);
	bool	Compile(const char* vert, const char* frag);
	void	PreLink();
	void	Link();
private:
	GLuint  _program;
	GLint	_uniforms[eUniform_Count];
	lfStr	_name;
	GLuint	_vert;
	GLuint	_frag;
};

#endif


