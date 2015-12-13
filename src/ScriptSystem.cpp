#include "ScriptSystem.h"
#include "sys/sys_public.h"
#include "common/String.h"

#include "luautils.h"

#include "Sprite.h"

int lua_print(lua_State * luastate)
{
	int nargs = lua_gettop(luastate);

	lfStr t;
	for (int i=1; i <= nargs; i++)
	{
		if (lua_istable(luastate, i))
			t += "table";
		else if (lua_isnone(luastate, i))
			t += "none";
		else if (lua_isnil(luastate, i))
			t += "nil";
		else if (lua_isboolean(luastate, i))
		{
			if (lua_toboolean(luastate, i) != 0)
				t += "true";
			else
				t += "false";
		}
		else if (lua_isfunction(luastate, i))
			t += "function";
		else if (lua_islightuserdata(luastate, i))
			t += "lightuserdata";
		else if (lua_isthread(luastate, i))
			t += "thread";
		else
		{
			const char * str = lua_tostring(luastate, i);
			if (str)
				t += lua_tostring(luastate, i);
			else
				t += lua_typename(luastate, lua_type(luastate, i));
		}
		if (i!=nargs)
			t += "\t";
	}
	Sys_Printf("%s\n", t.c_str());

	return 0;
}





ScriptSystem::ScriptSystem()
{
}

ScriptSystem::~ScriptSystem()
{
}

bool ScriptSystem::Init()
{
	L = lua_open();
	luaL_openlibs(L);

	// Register our version of the global "print" function
	const luaL_reg global_functions [] = {
		{"print", lua_print},
		{nullptr, nullptr}
	};
	luaL_register(L, "_G", global_functions);

	luaopen_sprite(L);
	luaopen_render(L);
	luaopen_model(L);
	luaopen_camera(L);
	luaopen_animodel(L);

	return true;
}

bool ScriptSystem::RunScript( const char* filename )
{
	int err = luaL_dofile(L, filename);
	if(err != 0)
		Sys_Error("script system call %s error code %d\n %s", filename, err, lua_tostring(L, -1));

	return true;
}


bool ScriptSystem::Call( const char* funcname )
{
	lua_getglobal(L, funcname);
	int err = lua_pcall(L, 0, 0, 0);
	if (err != 0)
	{
		Sys_Error("script system call %s error code %d", funcname, err);
	}
	return false;
}

lua_State* ScriptSystem::GetLuaState()
{
	return L;
}

void ScriptSystem::CallFuncI( const char* funcname, double i )
{
	lua_getglobal(L, funcname);  /* function to be called */
	lua_pushnumber(L, i);   /* push 1st argument */
    
	if (lua_pcall(L, 1, 0, 0) != 0)
		luaL_error(L, "error running function `f': %s", lua_tostring(L, -1));
}
