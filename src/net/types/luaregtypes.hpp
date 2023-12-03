/**
 * @file luaregtypes.hpp
 * @author Mathieu Cayeux
 * @brief Utility for loading types lua classes
 * @version 0.1
 * @date 2023-12-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_LUAREGTYPES_H
#define MINESERVER_LUAREGTYPES_H

#include <plugins/luaheaders.h>
#include <net/types/clientstate.h>
#include <net/types/chatmessage.h>

/**
 * @brief Loads types classes to lua
 *
 * @param state the lua state to load to
 */
void loadTypesLua(lua_State *state)
{
    const char* namespaceName = "types";
    loadClientStateLua(state, namespaceName);
    ChatMessage::loadLua(state, namespaceName);
}

#endif // MINESERVER_LUAREGTYPES_H