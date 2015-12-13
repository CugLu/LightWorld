#include "Shader.h"
#include "sys/sys_public.h"

const char* UniformType[16] = 
{
	"WVP",
	"COLOR",
	"texture1",
	"modelView",
	"invModelView",
	"fvEyePosition",
	"fvLightPosition",
	"bumpMap"
};

const char* AttribType[16] = 
{
	"vPosition",
	"vTexCoord",
	"vNormal",
	"vTangent",
	"vBinormal"
};


Shader::Shader()
{
	memset(_uniforms, -1, eUniform_Count);
}

void Shader::GetUniformLocation( unformType_t type )
{
	if (type >= 0 && type <= eUniform_Count)
	{
		_uniforms[type] = glGetUniformLocation(_program, UniformType[type]);
		//Sys_Printf("%d, %d, %s \n", _uniforms[type], type, UniformType[type]);
	}
}

void Shader::BindAttribLocation( attribType_t type )
{
	if (type >= 0 && type < eAttrib_Count)
		glBindAttribLocation(_program, type, AttribType[type]);
}

GLuint Shader::GetUniform( unformType_t type )
{
	if (type >= 0 && type < eUniform_Count && _uniforms[type] >= 0)
		return _uniforms[type];

	Sys_Error("Get uniform type %s %d %s %d", _name.c_str(), type, UniformType[type],  _uniforms[type]);
	return -1;
}

GLuint Shader::GetProgarm()
{
	return _program;
}

bool Shader::LoadFromFile( const char* vfile, const char* ffile )
{
	_program = GL_CreateProgramFromFile(vfile, ffile);
	return false;
}

bool Shader::LoadFromBuffer( const char* vfile, const char* ffile )
{
	_program = GL_CreateProgram(vfile, ffile);
	return true;
}

bool Shader::SetName( const char* name )
{
	_name = name;
	return true;
}

