#include "luautils.h"

void Lua_PushFunction (lua_State* L, const char* name, lua_CFunction func)
{
    lua_pushstring(L,name);
    lua_pushcfunction(L,func);
    lua_rawset(L,-3);
}

void Lua_SetMetatable( lua_State* L, const char* metaname ) 
{
	luaL_getmetatable(L, metaname);                    
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		luaL_error(L, "get metatable %s failed!", metaname);
		return;
	}

	lua_setmetatable(L, -2);  
}

void Lua_PushVec3( lua_State* L, float x, float y, float z )
{
	lua_createtable(L, 0, 0);
	lua_pushnumber(L, x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, y);
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, z);
	lua_setfield(L, -2, "z");
}

void Lua_PushVec2( lua_State* L, float x, float y )
{
	lua_createtable(L, 0, 0);
	lua_pushnumber(L, x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, y);
	lua_setfield(L, -2, "y");
}
