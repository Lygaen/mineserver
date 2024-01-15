/**
 * @file luaregtypes.hpp
 * @author Lygaen
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
#include <types/clientstate.h>
#include <types/chatmessage.h>
#include <types/angle.hpp>
#include <types/vector.hpp>
#include <types/uuid.h>

/**
 * @brief Loads types classes to lua
 *
 * @param state the lua state to load to
 */
void loadTypesLua(lua_State *state)
{
    const char *namespaceName = "types";

    UUID::loadLua(state, namespaceName);
    loadClientStateLua(state, namespaceName);
    ChatMessage::loadLua(state, namespaceName);
    Angle::loadLua(state, namespaceName);
    loadVectorLua(state, namespaceName);
}

#endif // MINESERVER_LUAREGTYPES_H