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
#include <net/packets/login/loginstartend.h>
#include <net/packets/login/encryptionexchange.h>
#include <net/packets/login/setcompression.h>
#include <net/packets/play/disconnect.h>
#include <plugins/events/clientevents.hpp>
#include <plugins/event.h>

Client::Client(ClientSocket sock) : sock(sock), stream(new NetSocketStream(sock))
{
    ClientConnectedEvent connectedEvent;
    EventsManager::inst()->fire(connectedEvent);
}

Client::~Client()
{
    if (!stream)
        return;
    delete stream;
}

void Client::loop()
{
    int32_t len = stream->readVarInt();
    if (len == 0xFE && state == ClientState::HANDSHAKE)
    {
        close("Connection prior to 1.7");
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
            close("Invalid handshake protocol");
            return;
        }

        HandshakePacket handshake;
        handshake.read(stream);

        state = handshake.nextState;

        // Invalid state given !
        if ((state != ClientState::STATUS && state != ClientState::LOGIN) ||
            // Login but invalid protocol version
            (state == ClientState::LOGIN && handshake.protocolVersion != MC_VERSION_NUMBER))
            close("Invalid state or invalid protocol version");
        break;
    }
    case ClientState::STATUS:
    {
        switch (id)
        {
        case 0x00:
        {
            logger::debug("Initiating Server List Ping...");
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
            close("Ping Protocol finished");
            return;
        }
        default:
        {
            close("Invalid client response");
            return;
        }
        }
        break;
    }
    case ClientState::LOGIN:
    {
        switch (id)
        {
        case 0x00:
        {
            LoginStart loginStart;
            loginStart.read(stream);

            player.name = loginStart.name;
            if (!Config::inst()->ONLINE_MODE.getValue())
            {
                player.uuid = UUID::newRandom();
                initiatePlayerJoin();
                return;
            }

            verifyToken = crypto::randomSecure(sizeof(verifyToken));

            EncryptionRequest request(verifyToken.get(), sizeof(verifyToken));
            request.send(stream);
            break;
        }
        case 0x01:
        {
            EncryptionResponse response;
            response.read(stream);

            if (response.verifyTokenLength != sizeof(verifyToken) ||
                !std::equal(response.verifyToken.get(), response.verifyToken.get() + response.verifyTokenLength, verifyToken.get()))
            {
                close("Invalid verify token");
                return;
            }
            stream = new CipherStream(stream, response.sharedSecret.get(), response.sharedSecret.get());

            crypto::MinecraftHash hash;
            hash.update("");
            hash.update(std::string((const char *)response.sharedSecret.get(), response.sharedSecretLength));

            int outLen;
            std::unique_ptr<std::byte[]> b = crypto::getPublicRSAKey(&outLen);
            hash.update(std::string((const char *)b.get(), outLen));

            mojangapi::HasJoinedResponse hasJoined;
            if (Config::inst()->PREVENT_PROXY_CONNECTIONS.getValue() && !sock.isLocal())
            {
                hasJoined = mojangapi::hasJoined(player.name, hash.finalize(), sock.getAddress());
            }
            else
            {
                hasJoined = mojangapi::hasJoined(player.name, hash.finalize());
            }

            if (player.name != hasJoined.name)
                close("Invalid joining name");

            player.uuid = hasJoined.id;

            initiatePlayerJoin();
            return;
        }
        default:
        {
            close("Invalid client response");
            return;
        }
        }
        break;
    }
    case ClientState::PLAY:
    {
        close("Not yet implemented");
        return;
    }
    }
}

void Client::initiatePlayerJoin()
{
    if (Config::inst()->COMPRESSION_LVL.getValue() != 0 && !sock.isLocal())
    {
        SetCompression comp(Config::inst()->COMPRESSION_THRESHOLD.getValue());
        comp.send(stream);

        stream = new ZLibStream(stream, Config::inst()->COMPRESSION_LVL.getValue(), Config::inst()->COMPRESSION_THRESHOLD.getValue());
    }

    LoginSuccess loginSuccess(player.name, player.uuid);
    loginSuccess.send(stream);

    state = ClientState::PLAY;

    logger::debug("Player %s (%s) has joined the server !", player.name.c_str(), player.uuid.getFull().c_str());
    close("Not yet implemented");
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
        close(err.what());
    }
}

void Client::close(const std::string &reason)
{
    isRunning = false;

    if (state == ClientState::LOGIN)
    {
        DisconnectLogin disconnect(reason);
        disconnect.send(stream);
    }
    else if (state == ClientState::PLAY)
    {
        DisconnectPlay disconnect(reason);
        disconnect.send(stream);
    }

    // sock.close();
    if (reason != "")
        logger::debug("Connection closed : %s", reason.c_str());
}
