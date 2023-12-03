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
    int luaCustomPrint(lua_State *state);

    /**
     * @brief Loads default libs
     *
     * Opens all default libs and creates a custom print
     * luaCustomPrint(lua_State*) with the name pointer
     * that should be pointing to the plugin name.
     * @param state lua state to load libs to
     * @param name the pointer to the name of the plugin
     */
    void registerDefaultLibs(lua_State *state, const char *name);
};

#endif // MINESERVER_LUAHEADERS_H