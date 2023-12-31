/**
 * @file disconnect.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_DISCONNECT_H
#define MINESERVER_DISCONNECT_H

#include <net/packet.h>

/**
 * @brief Disconnect Packet for Play state
 *
 * Packet that should be sent to disconnect
 * a client when being in ClientState::PLAY
 * state.
 */
class DisconnectPlay : public IPacket
{
private:
    void write(IMCStream *stream) override;

public:
    /**
     * @brief Construct a new Disconnect Play object
     *
     * @param reason the reason for the disconnection
     */
    DisconnectPlay(const ChatMessage &reason) : IPacket(0x40), reason(reason) {}

    /**
     * @brief The reason for the disconnection
     *
     */
    ChatMessage reason;

    /**
     * @brief Reads the packet from the stream
     *
     * @param stream the stream to read from
     * @deprecated should not be used, useless
     */
    void read(IMCStream *stream) override;

    // TODO implement everything in Lua
};

/**
 * @brief Disconnect Packet for Login state
 *
 * Packet that should be sent to disconnect
 * a client when being in ClientState::LOGIN
 * state.
 */
class DisconnectLogin : public IPacket
{
private:
    void write(IMCStream *stream) override;

public:
    /**
     * @brief Construct a new Disconnect Login object
     *
     * @param reason the reason for the disconnection
     */
    DisconnectLogin(const ChatMessage &reason) : IPacket(0x00), reason(reason) {}

    /**
     * @brief the reason for the disconnection
     *
     */
    ChatMessage reason;

    /**
     * @brief Reads the packet from the stream
     *
     * @param stream the stream to read from
     * @deprecated should not be used, useless
     */
    void read(IMCStream *stream) override;

    // TODO implement everything in Lua
};

#endif // MINESERVER_DISCONNECT_H