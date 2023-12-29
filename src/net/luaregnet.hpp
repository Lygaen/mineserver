/**
 * @file luaregnet.hpp
 * @author Lygaen
 * @brief Utility for loading network lua classes
 * @version 0.1
 * @date 2023-12-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_LUAREGNET_H
#define MINESERVER_LUAREGNET_H

#include <plugins/luaheaders.h>
#include <net/packets/luaregpackets.hpp>
#include <types/luaregtypes.hpp>

/**
 * @brief Loads network lua classes
 *
 * @param state the lua state to load to
 */
void loadNetLua(lua_State *state)
{
    loadTypesLua(state);
    loadPacketsLua(state);
}

#endif // MINESERVER_LUAREGNET_H