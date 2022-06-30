#include "client.h"
#include <utils/logger.h>
#include <data/packets/handshake/handshake.h>
#include <data/packets/status/statusresponse.h>
#include <data/packets/status/pingpong.h>
#include <data/packets/login/loginstatus.h>

Client::Client(socket_t socket) : socket(socket), streamHolder(new SocketStream(socket)),
                                  stream(*streamHolder), state(State::HANDSHAKE), player("", "")
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
        catch (std::runtime_error &e)
        {
            // same as well
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
        state = handshake.nextState == 1 ? State::STATUS : State::LOGIN;
        break;
    }
    case State::STATUS:
    {
        switch (id)
        {
        case 0x00:
        {
            StatusResponse sr;
            sr.send(stream);
            break;
        }
        case 0x01:
        {
            PingPong pingPong;
            pingPong.read(stream);
            pingPong.send(stream);
            stop();
            break;
        }
        default:
            stop();
            break;
        }
        break;
    }
    case State::LOGIN:
    {
        switch (id)
        {
        case 0x00:
        {
            LoginStart start;
            start.read(stream);

            player = Player("", start.playerName);
            // Encryption request
            break;
        }
        case 0x01:
        {
            // Encryption response
            // Enable encryption
            LoginSuccess success(player.getUUID(), player.getUsername());
            success.send(stream);
        }
        }
        break;
    }

    default:
        stop();
        break;
    }

    stream.flush();
}
