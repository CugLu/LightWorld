#include "../luautils.h"
#include "../Image.h"
static int ImageGetType(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetType'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetType();
            lua_pushnumber(L, ret);
    }

    return 0;
}

static int ImageGetHeight(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetHeight'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetHeight();
            lua_pushnumber(L, ret);
    }

    return 0;
}


int lua_register_Image(lua_State* L)
{
    if (luaL_newmetatable(L, "Image")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
       Lua_PushFunction(L, "GetType", ImageGetType);
       Lua_PushFunction(L, "FreeData", ImageFreeData);
       Lua_PushFunction(L, "GetDepth", ImageGetDepth);
       Lua_PushFunction(L, "IsVolume", ImageIsVolume);
       Lua_PushFunction(L, "GetFaces", ImageGetFaces);
       Lua_PushFunction(L, "SetImage", ImageSetImage);
       Lua_PushFunction(L, "GetHeight", ImageGetHeight);
       Lua_PushFunction(L, "GetLevel", ImageGetLevel);
       Lua_PushFunction(L, "GetInternalFormat", ImageGetInternalFormat);
       Lua_PushFunction(L, "GetWidth", ImageGetWidth);
       Lua_PushFunction(L, "GetImageSize", ImageGetImageSize);
       Lua_PushFunction(L, "GetMipLevels", ImageGetMipLevels);
       Lua_PushFunction(L, "IsCompressed", ImageIsCompressed);
       Lua_PushFunction(L, "FlipSurface", ImageFlipSurface);
       Lua_PushFunction(L, "IsCubeMap", ImageIsCubeMap);
       Lua_PushFunction(L, "GetFormat", ImageGetFormat);
       Lua_PushFunction(L, "ConvertCrossToCubemap", ImageConvertCrossToCubemap);

    }
    return 1;
}
