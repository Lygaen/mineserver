#include "luaheaders.h"

void lua::registerDefaultLibs(lua_State *state)
{
    luaL_openlibs(state);
    lua_register(state, "print", luaCustomPrint);
}
