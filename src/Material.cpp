#include "Material.h"
#include "common/Lexer.h"
#include "sys/sys_public.h"
#include "Shader.h"

Material::Material() :_hasColor(false), 
					  _hasTexture(false){

}

Material::~Material() {
	if (_vert)
		delete[] _vert;

	if (_frag)
		delete[] _frag;

}

bool Material::LoadMemory( const char* buffer ) {
	_numAttri = 0;

	Lexer lexer;
	lexer.LoadMemory(buffer);

	Token tk;
	while(lexer.Lex(tk)) {
		if (tk._data == "vert")
		{
			ParseVertProgram(lexer);
		}
		else if (tk._data == "frag")
		{
			ParseFragProgram(lexer);
		}
		else
		{
			Sys_Error("error %s", tk.Name(), tk._data.c_str());
		}
	}

	_shader.Compile(_vert, _frag);
	_shader.PreLink();

	if (_hasPosition)
		_shader.BindAttribLocation(eAttrib_Position);

	if (_hasColorAttr)
		_shader.BindAttribLocation(eAttrib_Color);

	_shader.Link();


	if (_hasWorldViewPorj)
		_shader.GetUniformLocation(eUniform_MVP);

	if (_hasColor)
		_shader.GetUniformLocation(eUniform_Color);

	if (_hasTexture)
		_shader.GetUniformLocation(eUniform_Samper0);

	Sys_Printf("material: %s\n"
		"          attri:%d %d %d %d %d"
			  "          color: %s\n" 
			  "          texture: %s\n",
			  _name.c_str(),
			  _attriArr[0], _attriArr[1], _attriArr[2], _attriArr[3], _attriArr[4],
			   _hasColor? "true" : "false", 
			  _hasTexture? "true" : "false");
	return false;
}

bool Material::HasPosition() {
	return false;
}

bool Material::HasTexCoord() {
	return false;
}

bool Material::HasNormal() {
	return false;
}

bool Material::HasColor() {
	return false;
}

unsigned int Material::ProgramId() {
	return 11;
}

bool Material::ParseVertProgram( Lexer& lexer ) {
	int openParen = 0;
	int start = lexer.CurrentPos();
	Token tk;
	while (lexer.Lex(tk))
	{
		if (tk._data == "vPosition")
			AddAttri(eAttrib_Position);
		else if(tk._data == "vTexCoord")
			AddAttri(eAttrib_TexCoord);
		else if(tk._data == "vNormal")
			AddAttri(eAttrib_Normal);
		else if (tk._data == "vTangent")
			AddAttri(eAttrib_Tangent);
		else if (tk._data == "vBinormal")
			AddAttri(eAttrib_Binormal);
		else if (tk._data == "vColor") {
			_hasColorAttr = true;
			AddAttri(eAttrib_Color);
		}
		else if (tk._data == "WVP")
			_hasWorldViewPorj = true;
		else if (tk._data == "modelView")
			_hasModelView = true;
		else if (tk._data == "invModelView")
			_hasInvModelView = true;

		else if (tk._type == '{')
		{
			openParen ++;
		}
		else if (tk._type == '}')
		{
			openParen--;
			if (openParen < 0)
			{
				_vert = lexer.SubStr(start, lexer.CurrentPos()-1);
				return true;
			}
		}
	}
	return false;
}

bool Material::ParseFragProgram( Lexer& lexer ) {
		int openParen = 0;
	int start = lexer.CurrentPos();
	Token tk;
	while (lexer.Lex(tk))
	{
		if (tk._data == "WVP")
			_hasWorldViewPorj = true;
		else if (tk._data == "COLOR")
			_hasColor = true;
		else if (tk._data == "texture1")
			_hasTexture = true;
		else if (tk._data == "modelView")
			_hasModelView = true;
		else if (tk._data == "invModelView")
			_hasInvModelView = true;
		else if (tk._type == '{')
		{
			openParen ++;
		}
		else if (tk._type == '}')
		{
			openParen--;
			if (openParen < 0)
			{
				_frag = lexer.SubStr(start, lexer.CurrentPos()-1);
				return true;
			}
		}
	}
	return false;
}

void Material::SetName( const char* name )
{
	_name = name;
}

void Material::AddAttri( int type )
{
	for (int i=0; i<_numAttri; i++)
		if (_attriArr[i] == type)
			return;

	_attriArr[_numAttri++] = type;
}

