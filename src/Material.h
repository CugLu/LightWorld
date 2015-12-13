#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "glutils.h"
#include "Shader.h"

#define MAX_ATTRI 9

class Lexer;

class Material
{
public:
	Material();
	~Material();

	bool LoadMemory(const char* buffer);

	bool HasPosition();

	bool HasTexCoord();

	bool HasNormal();

	bool HasColor();

	bool ParseVertProgram(Lexer& lexer);

	bool ParseFragProgram(Lexer& lexer);

	unsigned int ProgramId();

	void SetName(const char* name);
public:
	lfStr _name;

	bool _hasPosition;
	bool _hasTexCoord;
	bool _hasNormal;
	bool _hasTangent;
	bool _hasBinormal;
	bool _hasWorldViewPorj;
	bool _hasColor;
	bool _hasTexture;
	bool _hasModelView;
	bool _hasInvModelView;
	bool _hasEyePosition;
	bool _hasLightPosition;
	bool _hasBumpMap;

public:
	unsigned short _attriArr[MAX_ATTRI];
	unsigned short _numAttri;

	char* _vert;
	char* _frag;

	Shader _shader;

};

#endif


