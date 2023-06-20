/**
 * @file serverlist.h
 * @author Mathieu Cayeux
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
     * @brief Construct a new Server List Packet object
     *
     */
    ServerListPacket() : IPacket(0x00) {}
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
};

#endif // MINESERVER_SERVERLIST_H