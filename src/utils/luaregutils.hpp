/**
 * @file luaregutils.hpp
 * @author Mathieu Cayeux
 * @brief Utility for loading utils lua classes
 * @version 0.1
 * @date 2023-12-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_LUAREGUTILS_H
#define MINESERVER_LUAREGUTILS_H

#include <plugins/luaheaders.h>

/**
 * @brief Loads utils classes to lua
 *
 * @param state the state to load to
 */
void loadUtilsLua(lua_State *state)
{
    Config::inst()->loadLuaLib(state);
}

#endif // MINESERVER_LUAREGUTILS_H