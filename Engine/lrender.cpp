#include "../luautils.h"
#include "../renderer\RenderSystem.h"
#include "../Sprite.h"
#include "../Model.h"
#include "../Camera.h"

static RenderSystem* get_render(lua_State* L) {
	lua_getglobal(L, "renderSys");
	return *reinterpret_cast<RenderSystem**>(lua_touserdata(L, -1));
}

__NEW_FUNC(Sprite)
__NEW_FUNC(Model)
__NEW_FUNC(Camera)
__NEW_FUNC(AniModel)

static int render_addsprite(lua_State* L){
	RenderSystem* cobj = get_render(L);
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'AddSprite'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        Sprite* arg0 = *reinterpret_cast<Sprite**>(lua_touserdata(L, 1));
        bool ret = cobj->AddSprite(arg0);
        lua_pushboolean(L, ret);
        return 1;
    }
    return 0;
}

static int rendersystem_addmodel(lua_State* L){
	RenderSystem* cobj = get_render(L);
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'AddModel'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        Model* arg0 = *reinterpret_cast<Model**>(lua_touserdata(L, 1));
        bool ret = cobj->AddModel(arg0);
        lua_pushboolean(L, ret);
        return 1;
    }
    return 0;
}

static int rendersystem_addanimodel(lua_State* L){
	RenderSystem* cobj = get_render(L);
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'AddModel'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        AniModel* arg0 = *reinterpret_cast<AniModel**>(lua_touserdata(L, 1));
        bool ret = cobj->AddAnimModel(arg0);
        lua_pushboolean(L, ret);
        return 1;
    }
    return 0;
}

static const luaL_Reg syslib[] = {
  {"newsprite", render_newSprite},
  {"newmodel", render_newModel},
  {"newcamera", render_newCamera},
  {"newanimodel", render_newAniModel},
  {"addsprite", render_addsprite},
  {"addmodel", rendersystem_addmodel},
  {"addanimodel", rendersystem_addanimodel},
  {NULL, NULL}
};

int luaopen_render(lua_State* L) {
	luaL_register(L, "render", syslib);
    return 1;
}
