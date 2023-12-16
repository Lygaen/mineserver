/**
 * @file pingpong.h
 * @author Lygaen
 * @brief The file containing logic for pinging the server
 * @version 1.0
 * @date 2023-06-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_PINGPONG_H
#define MINESERVER_PINGPONG_H

#include <net/packet.h>
#include <plugins/luaheaders.h>

/**
 * @brief The Ping Pong Packet
 *
 * Packet for detecting the ping between
 * the client and the server.
 */
class PingPongPacket : public IPacket
{
protected:
    /**
     * @brief Write Packet Data
     *
     * Writes back the payload obtained
     * @param stream the stream to write to
     */
    void write(IMCStream *stream) override;

public:
    /**
     * @brief Construct a new Server List Packet object
     *
     */
    PingPongPacket() : IPacket(0x01), payload(0) {}
    /**
     * @brief Destroy the Server List Packet object
     *
     */
    ~PingPongPacket() = default;

    /**
     * @brief Payload
     *
     * Client's payload used to calculate ping
     */
    long payload;

    /**
     * @brief Read Packet Data
     *
     * Reads the payload from the client
     * @param stream the stream to read from
     */
    void read(IMCStream *stream) override;

    /**
     * @brief Loads this Packet as lua class
     *
     * @param state lua state to load to
     * @param baseNamespaceName the namespace name
     */
    static void loadLua(lua_State* state, const char* baseNamespaceName);
};

#endif // MINESERVER_PINGPONG_H