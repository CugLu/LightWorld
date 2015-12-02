#include "../luautils.h"
#include "../Sprite.h"

static int sprite_setlabel(lua_State* L){
    Sprite* cobj = *reinterpret_cast<Sprite**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'SetLabel'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        const char* arg0 = lua_tostring(L, 2);
        cobj->SetLabel(arg0);
    }

    return 0;
}

static int sprite_settexture(lua_State* L){
    Sprite* cobj = *reinterpret_cast<Sprite**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'SetTexture'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        const char* arg0 = lua_tostring(L, 2);
        cobj->SetTexture(arg0);
    }

    return 0;
}

static int sprite_getposition(lua_State* L){
    Sprite* cobj = *reinterpret_cast<Sprite**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'GetPosition'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) {
        vec3 ret = cobj->GetPosition();
        Lua_PushVec3(L, ret.x, ret.y, ret.z);
        return 1;
    }

    return 0;
}

static int sprite_setposition(lua_State* L){
    Sprite* cobj = *reinterpret_cast<Sprite**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'SetPosition'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 3) {
        float arg0 = lua_tonumber(L, 2);
        float arg1 = lua_tonumber(L, 3);
        float arg2 = lua_tonumber(L, 4);
        cobj->SetPosition(arg0, arg1, arg2);
    }

    return 0;
}

static int sprite_setviewproj(lua_State* L){
	Sprite* cobj = *reinterpret_cast<Sprite**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'SetViewProj'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        mat4* arg0 = (mat4*)lua_touserdata(L, 2);
        cobj->SetViewProj(arg0);
    }

    return 0;
}

int luaopen_sprite(lua_State* L)
{
    if (luaL_newmetatable(L, "Sprite")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        Lua_PushFunction(L, "setLabel", sprite_setlabel);
        Lua_PushFunction(L, "setTexture", sprite_settexture);
        Lua_PushFunction(L, "getPosition", sprite_getposition);
        Lua_PushFunction(L, "setPosition", sprite_setposition);
		Lua_PushFunction(L, "setViewProj", sprite_setviewproj);
    }
    return 1;
}
