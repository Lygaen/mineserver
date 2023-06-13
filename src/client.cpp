#include "client.h"
#include <utils/logger.h>
#include <net/packets/handshake.h>

Client::Client(ClientSocket sock) : sock(sock), stream(new NetSocketStream(sock))
{
}

Client::~Client()
{
    delete stream;
}

void Client::loop()
{
    int32_t len = stream->readVarInt();
    int32_t id = stream->readVarInt();

    logger::debug("C->S Len:%d Id:%d", len, id);

    switch (state)
    {
    case ClientState::HANDSHAKE:
    {
        HandshakePacket handshake;
        handshake.read(stream);

        state = handshake.nextState;

        logger::debug("Client handshake protocolVersion:%d serverAddress:%s serverPort:%d nextState:%d",
                      handshake.protocolVersion,
                      handshake.serverAddress.c_str(),
                      handshake.serverPort,
                      handshake.nextState);

        // Invalid state given !
        if ((state != ClientState::STATUS && state != ClientState::LOGIN) ||
            // Login but invalid protocol version
            (state == ClientState::LOGIN && handshake.protocolVersion != MC_VERSION_NUMBER))
            close();
    }
    case ClientState::STATUS:
    case ClientState::LOGIN:
    case ClientState::PLAY:
    {
        close();
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
        }
    }
    catch (const std::exception &err)
    {
        logger::error("Client ended badly : %s", err.what());
    }

    sock.close();
}

void Client::close()
{
    isRunning = false;
}
