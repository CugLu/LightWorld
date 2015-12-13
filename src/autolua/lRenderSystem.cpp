#include "../luautils.h"
#include "../renderer\RenderSystem.h"

static int rendersystem_adddrawsur(lua_State* L){
    RenderSystem* cobj = *reinterpret_cast<RenderSystem**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'AddDrawSur'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        drawSurf_t* arg0 = *reinterpret_cast<drawSurf_t**>(lua_touserdata(L, 2));
        bool ret = cobj->AddDrawSur(arg0);
        lua_pushboolean(L, ret);
        return 1;
    }

    return 0;
}

static int rendersystem_getnumsurf(lua_State* L){
    RenderSystem* cobj = *reinterpret_cast<RenderSystem**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'GetNumSurf'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) {
        int ret = cobj->GetNumSurf();
        lua_pushnumber(L, ret);
        return 1;
    }

    return 0;
}

static int rendersystem_addsprite(lua_State* L){
    RenderSystem* cobj = *reinterpret_cast<RenderSystem**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'AddSprite'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        Sprite* arg0 = *reinterpret_cast<Sprite**>(lua_touserdata(L, 2));
        bool ret = cobj->AddSprite(arg0);
        lua_pushboolean(L, ret);
        return 1;
    }

    return 0;
}

static int rendersystem_drawstring(lua_State* L){
    RenderSystem* cobj = *reinterpret_cast<RenderSystem**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'DrawString'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        const char* arg0 = lua_tostring(L, 2);
        cobj->DrawString(arg0);
    }

    return 0;
}

static int rendersystem_addmodel(lua_State* L){
    RenderSystem* cobj = *reinterpret_cast<RenderSystem**>(lua_touserdata(L, 1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'AddModel'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        Model* arg0 = *reinterpret_cast<Model**>(lua_touserdata(L, 2));
        bool ret = cobj->AddModel(arg0);
        lua_pushboolean(L, ret);
        return 1;
    }

    return 0;
}


int luaopen_rendersystem(lua_State* L)
{
    if (luaL_newmetatable(L, "RenderSystem")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        Lua_PushFunction(L, "addDrawSur", rendersystem_adddrawsur);
        Lua_PushFunction(L, "getNumSurf", rendersystem_getnumsurf);
        Lua_PushFunction(L, "addSprite", rendersystem_addsprite);
        Lua_PushFunction(L, "drawString", rendersystem_drawstring);
        Lua_PushFunction(L, "addModel", rendersystem_addmodel);
    }
    return 1;
}
