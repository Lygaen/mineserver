/**
 * @file setcompression.h
 * @author Lygaen
 * @brief The file containing set compression packet logic
 * @version 0.1
 * @date 2023-12-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_SETCOMPRESSION_H
#define MINESERVER_SETCOMPRESSION_H

#include <net/packet.h>

/**
 * @brief Set Compression Packet
 *
 */
class SetCompression : public IPacket
{
private:
    /**
     * @brief Write Packet Data
     *
     * @param stream the stream to write to
     */
    void write(IMCStream *stream) override;

public:
    /**
     * @brief Construct a new Login Start object
     *
     */
    SetCompression(int threshold) : IPacket(0x03), threshold(threshold) {}

    /**
     * @brief Threshold in size before compressing packets
     *
     */
    int threshold;

    /**
     * @brief Reads Packet data
     *
     * @param stream the stream to read from
     * @deprecated should not be used, useless
     */
    void read(IMCStream *stream) override;

    // TODO implement everything in Lua
};

#endif // MINESERVER_SETCOMPRESSION_H