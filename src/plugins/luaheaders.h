/**
 * @file luaheaders.h
 * @author Mathieu Cayeux
 * @brief Utility header file for lua things
 * @version 0.1
 * @date 2023-11-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_LUAHEADERS_H
#define MINESERVER_LUAHEADERS_H

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include <LuaBridge/LuaBridge.h>
#include <utils/logger.h>

#ifndef DOXYGEN_IGNORE_THIS
namespace logger
{
    void plugin(const char *format, ...);
};
#endif

/**
 * @brief Lua utility namespace
 *
 */
namespace lua
{
    /**
     * @brief Custom print
     *
     * Wrapper around logger::plugin so that lua
     * doesn't directly print to console
     * @param state the lua state
     * @return int lua thing whether it succeeded
     */
    inline int luaCustomPrint(lua_State *state)
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

    /**
     * @brief Register default lua libs
     *
     * @param state the lua state
     */
    void registerDefaultLibs(lua_State *state, const char *name);
};

#endif // MINESERVER_LUAHEADERS_H