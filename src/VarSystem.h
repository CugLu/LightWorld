#ifndef __VARSYSTEM_H__
#define __VARSYSTEM_H__

#include "common/Array.h"
#include "common/String.h"

class Var
{
public:
	Var();
	~Var();

	int GetInt();
	bool GetBool();

private:
	lfStr value;
};


class VarSystem
{
public:
	VarSystem();
	~VarSystem();

	bool Init();

	bool Regster(Var* var);

private:

	Array<Var*> list;
};

extern VarSystem* varSystem;

#endif // !__SCRIPTSYSTEM_H__



