#include "luautils.h"
#include "Model.h"

static int model_setviewproj(lua_State* L){
    Model* cobj = *reinterpret_cast<Model**>(lua_touserdata(L, 1));
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

static int model_setposition(lua_State* L){
    Model* cobj = *reinterpret_cast<Model**>(lua_touserdata(L, 1));
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

static int model_getposition(lua_State* L){
    Model* cobj = *reinterpret_cast<Model**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'GetPosition'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) {
        Vec3 ret = cobj->GetPosition();
        Lua_PushVec3(L, ret.x, ret.y, ret.z);
        return 1;
    }

    return 0;
}

static int model_setfile(lua_State* L){
    Model* cobj = *reinterpret_cast<Model**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'SetFile'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        const char* arg0 = lua_tostring(L, 2);
        cobj->SetFile(arg0);
    }

    return 0;
}


int luaopen_model(lua_State* L)
{
    if (luaL_newmetatable(L, "Model")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        Lua_PushFunction(L, "setViewProj", model_setviewproj);
        Lua_PushFunction(L, "setPosition", model_setposition);
        Lua_PushFunction(L, "getPosition", model_getposition);
        Lua_PushFunction(L, "setFile", model_setfile);
    }
    return 1;
}
