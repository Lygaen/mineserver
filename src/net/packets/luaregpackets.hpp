/**
 * @file luaregpackets.hpp
 * @author Mathieu Cayeux
 * @brief Utility for loading packets lua classes
 * @version 0.1
 * @date 2023-12-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef MINESERVER_LUAREGPACKETS_H
#define MINESERVER_LUAREGPACKETS_H

#include <plugins/luaheaders.h>
#include <net/packets/status/serverlist.h>
#include <net/packets/status/pingpong.h>
#include <net/packets/handshake.h>

/**
 * @brief Loads packets lua classes
 *
 * @param state the lua state to load to
 */
void loadPacketsLua(lua_State *state)
{
    const char* namespaceName = "packets";
    ServerListPacket::loadLua(state, namespaceName);
    PingPongPacket::loadLua(state, namespaceName);
    HandshakePacket::loadLua(state, namespaceName);
}

#endif // MINESERVER_LUAREGPACKETS_H