/**
 * @file clientstate.h
 * @author Lygaen Cayeuyx
 * @brief The file containing just the enum of client state
 * @version 1.0
 * @date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 * I mean, this file is not really useful
 * and one file for just that is too much.
 * But I can't be bothered with self-referencing
 * files and other stuffs because or else this
 * enum will be defined in client.h, but is
 * used in packets that are referenced in client.h.
 * The compiler would not know what to do and fail.
 */

#ifndef MINESERVER_CLIENTSTATE_H
#define MINESERVER_CLIENTSTATE_H

#include <plugins/luaheaders.h>

/**
 * @brief Client State
 *
 * The enum for the interger representation
 * of the current client state.
 */
enum ClientState
{
    /**
     * @brief Handshake state
     *
     * The default state when a client
     * connects and has not yet handshaked.
     * The next state should be ::STATUS or ::LOGIN.
     */
    HANDSHAKE = 0,
    /**
     * @brief Status state
     *
     * The state when the client and server exchange
     * packets for server status.
     * The server should close connection afterwards.
     */
    STATUS = 1,
    /**
     * @brief Login state
     *
     * The state when the client and server authenticate
     * if needed.
     * The next state should be ::PLAY.
     */
    LOGIN = 2,
    /**
     * @brief Play state
     *
     * The state when the client is actively playing
     * on the server.
     */
    PLAY = 3
};

#ifndef DOXYGEN_IGNORE_THIS
template <>
struct luabridge::Stack<ClientState> : luabridge::Enum<ClientState,
        ClientState::HANDSHAKE,
        ClientState::STATUS,
        ClientState::LOGIN,
        ClientState::PLAY>
{
};
#endif

/**
 * @brief Loads ClientState to lua
 *
 * @param state lua state to load to
 * @param namespaceName the namespace to load to
 */
inline void loadClientStateLua(lua_State* state, const char* namespaceName) {
    luabridge::getGlobalNamespace(state)
        .beginNamespace(namespaceName)
        .beginNamespace("ClientState")
            .addProperty("HANDSHAKE", +[] {return ClientState::HANDSHAKE;})
            .addProperty("STATUS", +[] {return ClientState::STATUS;})
            .addProperty("LOGIN", +[] {return ClientState::LOGIN;})
            .addProperty("PLAY", +[] {return ClientState::PLAY;})
        .endNamespace()
        .endNamespace();
}
#endif // MINESERVER_CLIENTSTATE_H