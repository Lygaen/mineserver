/**
 * @file handshake.h
 * @author Mathieu Cayeux
 * @brief The file containing handshake protocol
 * @version 1.0
 * @date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_HANDSHAKE_H
#define MINESERVER_HANDSHAKE_H

#include <net/types/clientstate.h>
#include <net/packet.h>
#include <plugins/luaheaders.h>

/**
 * @brief Handshake Packet
 *
 * First packet that is read by the server
 * from the client. No writing should be involved.
 */
class HandshakePacket : public IPacket
{
protected:
    /**
     * @brief Write Packet Data
     *
     * Should not be called ! Handshake packet
     * should only be read, not written to !
     * @param stream the stream to write to
     * @deprecated Do not call this function
     */
    void write(IMCStream *stream) override;

public:
    /**
     * @brief Construct a new Handshake Packet object
     *
     */
    HandshakePacket() : IPacket(0x00) {}
    /**
     * @brief Destroy the Handshake Packet object
     *
     */
    ~HandshakePacket() override = default;

    /**
     * @brief Protocol Version
     *
     * The protocol version of the client
     * connecting to the server.
     */
    int protocolVersion{};
    /**
     * @brief Server Address
     *
     * The server address that the client
     * used to connect to the server.
     */
    std::string serverAddress;
    /**
     * @brief Server Port
     *
     * The port that the client used to
     * connect to the server.
     */
    unsigned short serverPort{};
    /**
     * @brief Next State
     *
     * The next state that the client
     * is asking to do, 1 for status,
     * 2 for login.
     */
    ClientState nextState = ClientState::HANDSHAKE;

    /**
     * @brief Read Packet Data
     *
     * Reads handshake data from the stream
     * @param stream the stream to read from
     */
    void read(IMCStream *stream) override;

    /**
     * @brief Loads packet as lua class
     *
     * @param state lua state to load to
     * @param baseNamespaceName the base namespace name to load to
     */
    static void loadLua(lua_State* state, const char* baseNamespaceName);
};

#endif // MINESERVER_HANDSHAKE_H