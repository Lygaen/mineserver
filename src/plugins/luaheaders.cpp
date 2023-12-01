#include "luaheaders.h"

void lua::registerDefaultLibs(lua_State *state, const char *name)
{
    luaL_openlibs(state);

    lua_pushlightuserdata(state, const_cast<void *>(reinterpret_cast<const void *>(name)));
    lua_pushcclosure(state, luaCustomPrint, 1);
    lua_setglobal(state, "print");
}