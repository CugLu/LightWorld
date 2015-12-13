#include "luautils.h"
#include "Camera.h"

static int camera_getview(lua_State* L){
    Camera* cobj = *reinterpret_cast<Camera**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'GetView'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) {
        mat4* ret = cobj->GetView();
        lua_pushlightuserdata(L, ret);
        return 1;
    }

    return 0;
}

static int camera_rise(lua_State* L){
    Camera* cobj = *reinterpret_cast<Camera**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'Rise'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        float arg0 = lua_tonumber(L, 2);
        cobj->Rise(arg0);
    }

    return 0;
}

static int camera_setup2dcamera(lua_State* L){
    Camera* cobj = *reinterpret_cast<Camera**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'Setup2DCamera'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 2) {
        float arg0 = lua_tonumber(L, 2);
        float arg1 = lua_tonumber(L, 3);
        cobj->Setup2DCamera(arg0, arg1);
    }

    return 0;
}

static int camera_yaw(lua_State* L){
    Camera* cobj = *reinterpret_cast<Camera**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'Yaw'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        float arg0 = lua_tonumber(L, 2);
        cobj->Yaw(arg0);
    }

    return 0;
}

static int camera_rotatebyaxis(lua_State* L){
    Camera* cobj = *reinterpret_cast<Camera**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'RotateByAxis'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 4) {
        float arg0 = lua_tonumber(L, 2);
        float arg1 = lua_tonumber(L, 3);
        float arg2 = lua_tonumber(L, 4);
        float arg3 = lua_tonumber(L, 5);
        cobj->RotateByAxis(Vec3(arg0, arg1, arg2), arg1);
    }

    return 0;
}

static int camera_walk(lua_State* L){
    Camera* cobj = *reinterpret_cast<Camera**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'Walk'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        float arg0 = lua_tonumber(L, 2);
        cobj->Forward(arg0);
    }

    return 0;
}

static int camera_getposition(lua_State* L){
    Camera* cobj = *reinterpret_cast<Camera**>(lua_touserdata(L, 1));
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

static int camera_getviewproj(lua_State* L){
    Camera* cobj = *reinterpret_cast<Camera**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'GetViewProj'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) {
        mat4* ret = cobj->GetViewProj();
        lua_pushlightuserdata(L, ret);
        return 1;
    }

    return 0;
}

static int camera_getproj(lua_State* L){
    Camera* cobj = *reinterpret_cast<Camera**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'GetProj'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) {
        mat4* ret = cobj->GetProj();
		lua_pushlightuserdata(L, ret);
        return 1;
    }

    return 0;
}

static int camera_setposition(lua_State* L){
    Camera* cobj = *reinterpret_cast<Camera**>(lua_touserdata(L, 1));
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

static int camera_setup3dcamera(lua_State* L){
    Camera* cobj = *reinterpret_cast<Camera**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'Setup3DCamera'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 2) {
        int arg0 = lua_tonumber(L, 2);
        int arg1 = lua_tonumber(L, 3);
        cobj->Setup3DCamera(arg0, arg1);
    }

    return 0;
}


int luaopen_camera(lua_State* L)
{
    if (luaL_newmetatable(L, "Camera")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        Lua_PushFunction(L, "getView", camera_getview);
        Lua_PushFunction(L, "rise", camera_rise);
        Lua_PushFunction(L, "setup2DCamera", camera_setup2dcamera);
        Lua_PushFunction(L, "yaw", camera_yaw);
        Lua_PushFunction(L, "rotateByAxis", camera_rotatebyaxis);
        Lua_PushFunction(L, "walk", camera_walk);
        Lua_PushFunction(L, "getPosition", camera_getposition);
        Lua_PushFunction(L, "getViewProj", camera_getviewproj);
        Lua_PushFunction(L, "getProj", camera_getproj);
        Lua_PushFunction(L, "setPosition", camera_setposition);
        Lua_PushFunction(L, "setup3DCamera", camera_setup3dcamera);
    }
    return 1;
}
