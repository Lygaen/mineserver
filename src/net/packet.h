/**
 * @file packet.h
 * @author Lygaen
 * @brief The file containing the packet data
 * @version 1.0
 * @date 2023-06-11
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_PACKET_H
#define MINESERVER_PACKET_H

#include <net/stream.h>

/**
 * @brief Interface for all Packets
 *
 * The sole interface that all packets should
 * inherit. Includes only the basics such as
 * the id and the basic functions.
 */
class IPacket
{
protected:
    /**
     * @brief Packet Id
     *
     * The id of the packet for Minecraft,
     * should be copied from https://wiki.vg (see 1.8.9)
     */
    int id;

    /**
     * @brief Write packet data to stream
     *
     * Writes raw packet data to a stream, excluding
     * id and length. If you want to send a packet
     * to a stream you should use IPacket#send() instead
     * Ideally only used internally and overriden by
     * children classes.
     * @param stream the stream to write to
     */
    virtual void write(IMCStream *stream) = 0;

    /**
     * @brief Construct a new IPacket object
     *
     * Everything is pretty much said.
     * @param id the Packet id, refer to \ref IPacket.id
     */
    IPacket(int id) : id(id)
    {
    }

public:
    /**
     * @brief Destroy the IPacket object
     *
     */
    virtual ~IPacket() = default;

    /**
     * @brief Reads data from the stream to a packet
     *
     * Reads corresponding data from the stream, interpreting
     * it to the packet format. Should be overriden when
     * creating a new subsequent class and can be used
     * anywhere on any stream, unlike IPackert#write()
     * @param stream the stream to read from
     */
    virtual void read(IMCStream *stream) = 0;
    /**
     * @brief Sends a Packet in Minecraft format
     *
     * Writes the packet data as well as id and length,
     * following minecraft's implementation of packeting.
     * Refer to https://wiki.vg for more information
     * as to their structure.
     * @param stream
     */
    void send(IMCStream *stream);
};

#endif // MINESERVER_PACKET_H