#include "client.h"
#include <utils/logger.h>
#include <data/packets/handshake/handshake.h>

Client::Client(socket_t socket) : socket(socket), streamHolder(new SocketStream(socket)),
                                  stream(*streamHolder), state(State::HANDSHAKE)
{
}

Client::~Client()
{
    cpnet_close(socket);
}

void Client::start()
{
    running = true;
    while (running)
    {
        try
        {
            loop();
        }
        catch (std::out_of_range &e)
        {
            // probably some client disconnecting
            break;
        }
    }
}

void Client::loop()
{
    int len = stream.readVarInt();
    stream.startRead(len);
    int id = stream.readVarInt();
    logger::packet(logger::PacketDirection::TO_SERVER, len, id, "NULL");

    switch (state)
    {
    case State::HANDSHAKE:
    {
        Handshake handshake;
        handshake.read(stream);
        logger::debug("PV {} ADDR {} PRT {} NS {}", handshake.protocolVersion,
                      handshake.serverAddress, handshake.port, handshake.nextState == 1 ? "STATUS" : "LOGIN");
        break;
    }
    default:
        break;
    }

    stream.flush();
}
