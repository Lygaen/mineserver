/**
 * @file clientevents.hpp
 * @author Lygaen
 * @brief The file containing all of the client events
 * @version 0.1
 * @date 2023-12-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_CLIENTEVENTS_H
#define MINESERVER_CLIENTEVENTS_H

#include <plugins/event.h>
#include <net/packets/status/serverlist.h>
#include <net/packets/handshake.h>

/**
 * @brief Event for a client connection
 *
 * Launched when a new clients connects.
 * Should implement in lua as such :
 * @code{.lua}
 * local function onClient(e)
 *     -- do your thing
 * end
 *
 * event.onClientConnected(onClient)
 * @endcode
 */
class ClientConnectedEvent : public IEvent<ClientConnectedEvent> {
};

/**
 * @brief Event for a client status
 *
 * Launched when a new clients was connected,
 * and we received it seems an handshake.
 * Should implement in lua as such :
 * @code{.lua}
 * local function onStatus(e)
 *     -- do your thing, like printing the client protocol version
 *     print("Client protocol version : " .. e.packet.protocolVersion)
 * end
 *
 * event.onClientHandshake(onStatus)
 * @endcode
 */
class ClientHandshakeEvent : public IEvent<ClientHandshakeEvent>
{
public:
    /**
     * @brief Pointer to handshake packet
     *
     * Pointer to the handshake packet,
     * so that lua and other listeners can modify
     * the data sent to clients individually.
     */
    HandshakePacket *packet;

    /**
     * @brief Construct a new Client Handshake Event object
     *
     * @param packet the pointer to the packet
     */
    ClientHandshakeEvent(HandshakePacket *packet) : packet(packet) {}

    /**
     * @brief Loads this event to lua
     *
     * @param state the lua state to load to
     */
    static void loadLua(lua_State *state)
    {
        luabridge::getGlobalNamespace(state)
            .beginNamespace("event")
            .addFunction("onClientHandshake", [](const luabridge::LuaRef &ref)
                         {
                    if(!ref.isFunction())
                        return;

                    EventsManager::inst()->subscribe<ClientHandshakeEvent>(const_cast<luabridge::LuaRef &>(ref)); })
            .beginClass<ClientHandshakeEvent>("ClientHandshake")
            .addProperty("packet", &ClientHandshakeEvent::packet)
            .endClass()
            .endNamespace();
    }
};

/**
 * @brief Event for a client status
 *
 * Launched when a new clients was connected,
 * it was a minecraft connection and it asked
 * for the status of the server.
 * Should implement in lua as such :
 * @code{.lua}
 * local function onStatus(e)
 *     -- do your thing, like random max players
 *     e.packet.maxPlayers = math.random(1,100)
 * end
 *
 * event.onClientStatus(onStatus)
 * @endcode
 */
class ClientStatusEvent : public IEvent<ClientStatusEvent>
{
public:
    /**
     * @brief Pointer to server list packet
     *
     * Pointer to the server list packet,
     * so that lua and other listeners can modify
     * the data sent to clients individually.
     */
    ServerListPacket *packet;

    /**
     * @brief Construct a new Client Status Event object
     *
     * @param packet the pointer to the packet
     */
    ClientStatusEvent(ServerListPacket *packet) : packet(packet) {}

    /**
     * @brief Loads this event to lua
     *
     * @param state the lua state to load to
     */
    static void loadLua(lua_State *state)
    {
        luabridge::getGlobalNamespace(state)
            .beginNamespace("event")
            .addFunction("onClientStatus", [](const luabridge::LuaRef &ref)
                         {
                    if(!ref.isFunction())
                        return;

                    EventsManager::inst()->subscribe<ClientStatusEvent>(const_cast<luabridge::LuaRef&>(ref)); })
            .beginClass<ClientStatusEvent>("ClientStatus")
            .addProperty("packet", &ClientStatusEvent::packet)
            .endClass()
            .endNamespace();
    }
};

#endif // MINESERVER_CLIENTEVENTS_H