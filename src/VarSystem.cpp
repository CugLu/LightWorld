#include "VarSystem.h"

static VarSystem local;
VarSystem* varSystem = &local;

Var::Var()
{
	varSystem->Regster(this);
}

Var::~Var()
{

}

int Var::GetInt()
{
	return 0;
}

bool Var::GetBool()
{
	return 1;
}

VarSystem::VarSystem()
{

}

VarSystem::~VarSystem()
{

}

bool VarSystem::Init()
{
	return false;
}

bool VarSystem::Regster(Var* var)
{
	list.push_back(var);
	return true;
}