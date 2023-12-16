/**
 * @file serverlist.h
 * @author Lygaen
 * @brief The file containing packet logic for server list
 * @version 1.0
 * @date 2023-06-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_SERVERLIST_H
#define MINESERVER_SERVERLIST_H

#include <net/packet.h>
#include <plugins/luaheaders.h>

/**
 * @brief Server List packet
 *
 * Packet for clients that load the
 * status data from the server such
 * as the MoTD, max players, online
 * players and so on.
 */
class ServerListPacket : public IPacket
{
protected:
    /**
     * @brief Write Packet Data
     *
     * Writes server list data to stream
     * @param stream the stream to write to
     */
    void write(IMCStream *stream) override;

public:
    /**
     * @brief Max players
     *
     * Max number of players allowed by the Config,
     * defaults to config.
     */
    int maxPlayers;
    /**
     * @brief Online players
     *
     * The number of online players at this time.
     * Defaults to actual number of players.
     */
    int onlinePlayers;
    /**
     * @brief Message of The Day (MoTD)
     *
     * Defaults to config.
     */
    ChatMessage motd;

    /**
     * @brief Construct a new Server List Packet object
     *
     */
    ServerListPacket();
    /**
     * @brief Destroy the Server List Packet object
     *
     */
    ~ServerListPacket() = default;

    /**
     * @brief Read Packet Data
     *
     * Reads server list data from the stream,
     * does nothing for the current protocol version.
     * @param stream the stream to read from
     * @deprecated No need to call it, it does nothing
     */
    void read(IMCStream *stream) override;

    /**
     * @brief Loads this Packet as a lua class
     *
     * @param state the state to load to
     * @param baseNamespaceName the namespace to load to
     */
    static void loadLua(lua_State* state, const char* baseNamespaceName);
};

#endif // MINESERVER_SERVERLIST_H