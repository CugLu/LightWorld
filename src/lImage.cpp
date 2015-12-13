#include "autolua.h"
#include "C:/program1/FlipEngine1/Engine\Image.h"
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

static int ImageFreeData(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'FreeData'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            cobj->FreeData();
    }

    return 0;
}

static int ImageGetDepth(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetDepth'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetDepth();
            lua_pushnumber(L, ret);
    }

    return 0;
}

static int ImageIsVolume(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'IsVolume'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            bool ret = cobj->IsVolume();
            lua_pushboolean(L, ret);
    }

    return 0;
}

static int ImageGetFaces(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetFaces'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetFaces();
            lua_pushnumber(L, ret);
    }

    return 0;
}

static int ImageSetImage(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'SetImage'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 5) 
    {
        int arg0 = lua_tonumber(L, 2);
        int arg1 = lua_tonumber(L, 3);
        int arg2 = lua_tonumber(L, 4);
        int arg3 = lua_tonumber(L, 5);
        const void* arg4 = (void*)lua_touserdata(L, 6);
            bool ret = cobj->SetImage(arg0, arg1, arg2, arg3, arg4);
            lua_pushboolean(L, ret);
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

static int ImageGetInternalFormat(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetInternalFormat'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetInternalFormat();
            lua_pushnumber(L, ret);
    }

    return 0;
}

static int ImageGetWidth(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetWidth'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetWidth();
            lua_pushnumber(L, ret);
    }

    return 0;
}

static int ImageGetImageSize(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetImageSize'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetImageSize();
            lua_pushnumber(L, ret);
    }
    if (argc == 1) 
    {
        int arg0 = lua_tonumber(L, 2);
            int ret = cobj->GetImageSize(arg0);
            lua_pushnumber(L, ret);
    }

    return 0;
}

static int ImageGetMipLevels(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetMipLevels'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetMipLevels();
            lua_pushnumber(L, ret);
    }

    return 0;
}

static int ImageIsCompressed(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'IsCompressed'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            bool ret = cobj->IsCompressed();
            lua_pushboolean(L, ret);
    }

    return 0;
}

static int ImageFlipSurface(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'FlipSurface'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            cobj->FlipSurface();
    }

    return 0;
}

static int ImageIsCubeMap(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'IsCubeMap'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            bool ret = cobj->IsCubeMap();
            lua_pushboolean(L, ret);
    }

    return 0;
}

static int ImageGetFormat(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetFormat'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetFormat();
            lua_pushnumber(L, ret);
    }

    return 0;
}

static int ImageConvertCrossToCubemap(lua_State* L)
{
    Image* cobj = (Image*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'ConvertCrossToCubemap'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            bool ret = cobj->ConvertCrossToCubemap();
            lua_pushboolean(L, ret);
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
