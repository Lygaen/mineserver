/**
 * @file client.cpp
 * @author Lygaen
 * @brief The file containing the logic for the client
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "client.h"
#include <utils/logger.h>
#include <net/packets/handshake.h>
#include <net/packets/status/serverlist.h>
#include <net/packets/status/pingpong.h>
#include <plugins/events/clientevents.hpp>
#include <plugins/event.h>

Client::Client(ClientSocket sock) : sock(sock), stream(new NetSocketStream(sock))
{
    ClientConnectedEvent connectedEvent;
    EventsManager::inst()->fire(connectedEvent);
}

Client::~Client()
{
    delete stream;
}

void Client::loop()
{
    int32_t len = stream->readVarInt();
    if (len == 254 && state == ClientState::HANDSHAKE)
    {
        logger::debug("Closing client connection - prior to 1.7");
        close();
        return;
    }

    int32_t id = stream->readVarInt();

    logger::debug("C->S Len:%d Id:%d", len, id);

    switch (state)
    {
    case ClientState::HANDSHAKE:
    {
        if (id != 0x00)
        {
            // wtf just tried to connect ??
            close();
            return;
        }

        HandshakePacket handshake;
        handshake.read(stream);

        state = handshake.nextState;

        // Invalid state given !
        if ((state != ClientState::STATUS && state != ClientState::LOGIN) ||
            // Login but invalid protocol version
            (state == ClientState::LOGIN && handshake.protocolVersion != MC_VERSION_NUMBER))
            close();

        if (state == ClientState::STATUS)
        {
            logger::debug("Initiating Server List Ping...");
        }
        else
        {
            logger::debug("Initiating Login...");
        }
        break;
    }
    case ClientState::STATUS:
    {
        switch (id)
        {
        case 0x00:
        {
            ServerListPacket serverlist;
            ClientStatusEvent statusEvent(&serverlist);
            EventsManager::inst()->fire(statusEvent);
            serverlist.send(stream);
            break;
        }
        case 0x01:
        {
            PingPongPacket pingpong;
            pingpong.read(stream);
            pingpong.send(stream);

            logger::debug("Finished Server List Ping !");
            close();
            return;
        }
        default:
        {
            close();
            return;
        }
        }
        break;
    }
    case ClientState::LOGIN:
    {
        switch (id)
        {
        }
        close();
    }
    case ClientState::PLAY:
    {
        close();
        return;
    }
    }
}

void Client::start()
{
    isRunning = true;
    state = ClientState::HANDSHAKE;

    try
    {
        while (isRunning)
        {
            loop();

            stream->flush();
        }
    }
    catch (const std::exception &err)
    {
        if (state == ClientState::HANDSHAKE)
            logger::error("Connection seems not minecrafty or protocol is too old");
        logger::error("Client ended badly : %s", err.what());
    }

    sock.close();
}

void Client::close()
{
    isRunning = false;
}
