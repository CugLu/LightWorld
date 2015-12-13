#ifndef __LUAUTILS_H__
#define __LUAUTILS_H__

extern "C"
{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
};

#define __TOLUAOBJ(TYPE) \
	TYPE* cobj = *reinterpret_cast<TYPE**>(lua_touserdata(L, 1)); \
    if (!cobj) { \
		luaL_error(L,"invalid 'cobj' in function 'GetPosition'", nullptr); \
		return 0; }\

#define __NEW_FUNC(TYPE) \
	static int render_new##TYPE(lua_State* L) {\
		TYPE** obj = (TYPE **)lua_newuserdata(L, sizeof(TYPE*));\
		*obj = new TYPE();\
		(*obj)->Init();\
		Lua_SetMetatable(L, #TYPE);\
		return 1;\
	}\


void Lua_PushFunction (lua_State* L, const char* name, lua_CFunction func);

void Lua_SetMetatable( lua_State* L, const char* metaname );

void Lua_PushVec3(lua_State* L, float x, float y, float z);

void Lua_PushVec2(lua_State* L, float x, float y);


// ---------------------------------------------------------------------------
int luaopen_sprite(lua_State* L);

int luaopen_render(lua_State* L);

int luaopen_model(lua_State* L);

int luaopen_camera(lua_State* L);

int luaopen_animodel(lua_State* L);

#endif


