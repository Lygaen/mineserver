/**
 * @file pingpong.h
 * @author Mathieu Cayeux
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
    void write(IStream *stream) override;

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

    long payload;

    /**
     * @brief Read Packet Data
     *
     * Reads the payload from the client
     * @param stream the stream to read from
     */
    void read(IStream *stream) override;
};

#endif // MINESERVER_PINGPONG_H