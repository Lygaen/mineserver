#include "client.h"
#include <utils/logger.h>

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

    if (id == 0)
    {
        logger::debug("Len %d Id %d", len, id);
        logger::debug("Protocol version %d", stream->readVarInt());
        logger::debug("Server Address %s", stream->readString().c_str());
        logger::debug("Server Port %d", stream->readUnsignedShort());
        logger::debug("Next State %d", stream->readVarInt());

        close();
    }
}

void Client::start()
{
    isRunning = true;

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
