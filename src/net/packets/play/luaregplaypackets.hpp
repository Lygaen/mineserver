/**
 * @file luaregplaypackets.hpp
 * @author Lygaen
 * @brief Utility for loading types lua classes
 * @version 0.1
 * @date 2024-01-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MINESERVER_LUAREGPLAYPACKETS_H
#define MINESERVER_LUAREGPLAYPACKETS_H

#include <plugins/luaheaders.h>
#include <net/packets/play/disconnect.h>

/**
 * @brief Loads entities classes to lua
 *
 * @param state the lua state to load to
 */
void loadPlayPacketsLua(lua_State *state)
{
    const char *namespaceName = "packets";

    DisconnectLogin::loadLua(state, namespaceName);
    DisconnectPlay::loadLua(state, namespaceName);
}

#endif // MINESERVER_LUAREGPLAYPACKETS_H