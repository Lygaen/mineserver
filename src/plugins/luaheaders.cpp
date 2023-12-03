#include "luaheaders.h"
#include <utils/logger.h>

int lua::luaCustomPrint(lua_State *state)
{
    const char *name = reinterpret_cast<const char *>(lua_touserdata(state, lua_upvalueindex(1)));
    int n = lua_gettop(state); /* number of arguments */
    int i;
    std::string endString;
    for (i = 1; i <= n; i++)
    { /* for each argument */
        size_t l;
        const char *s = luaL_tolstring(state, i, &l); /* convert it to string */
        if (i > 1)                                    /* not the first element? */
            endString += "\t";                        /* add a tab before it */
        endString += std::string(s, l);               /* print it */
        lua_pop(state, 1);                            /* pop result */
    }
    logger::plugin("[%s] %s", name, endString.c_str());
    return 0;
}

void lua::registerDefaultLibs(lua_State *state, const char *name)
{
    luaL_openlibs(state);

    lua_pushlightuserdata(state, const_cast<void *>(reinterpret_cast<const void *>(name)));
    lua_pushcclosure(state, luaCustomPrint, 1);
    lua_setglobal(state, "print");
}