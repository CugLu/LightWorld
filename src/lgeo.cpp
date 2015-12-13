#include "luautils.h"
#include "r_public.h"

static srfTriangles_s* lgeo_to(lua_State* L)
{
	srfTriangles_s* obj = *reinterpret_cast<srfTriangles_s**>(lua_touserdata(L, -1));
	Assert(obj);
	return obj;
}

static int lgeo_new(lua_State* L)
{
	srfTriangles_s** obj = (srfTriangles_s **)lua_newuserdata(L, sizeof(srfTriangles_s*));
	*obj = R_AllocStaticTriSurf();
	return 1;
}

static int lgeo_newVert(lua_State* L)
{
	srfTriangles_s* obj = lgeo_to(L);
	int num = lua_tonumber(L, -2);
	obj->numVerts = num;
	R_AllocStaticTriSurfVerts(obj, num);
	return 1;
}

static int lgeo_setVert(lua_State* L)
{
	srfTriangles_s* obj = lgeo_to(L);
	int idx = lua_tonumber(L, -2);
	float x = lua_tonumber(L, -3);
	float y = lua_tonumber(L, -4);
	float z = lua_tonumber(L, -5);
	obj->verts[idx].xyz.set(x, y, z);
	return 1;
}

static int lgeo_newIndice(lua_State* L)
{
	srfTriangles_s* obj = lgeo_to(L);
	int num = lua_tonumber(L, -2);
	obj->numIndexes = num;
	R_AllocStaticTriSurfIndexes(obj, num);
	return 1;
}

static int lgeo_setIndice(lua_State* L)
{
	srfTriangles_s* obj = lgeo_to(L);
	int idx = lua_tonumber(L, -2);
	float i = lua_tonumber(L, -3);
	obj->indexes[idx] = i;
	return 1;
}

