/**
 * @file luaregcmd.hpp
 * @author Lygaen
 * @brief The file for registering commands to lua
 * @version 0.1
 * @date 2024-02-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MINESERVER_LUAREGCMD_H
#define MINESERVER_LUAREGCMD_H

#include <plugins/luaheaders.h>
#include <cmd/commands.h>

/**
 * @brief Registers commands logic to lua
 *
 * @param state the state to load to
 */
void loadCmdLua(lua_State *state)
{
    const char *namespaceName = "cmd";

    ISender::loadLua(state, namespaceName);
    CommandsManager::loadLua(state, namespaceName);
}

#endif // MINESERVER_LUAREGCMD_H