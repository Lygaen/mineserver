#include "handshake.h"
#include <stdexcept>
#include <codecvt>
#include <locale>
#include <utils/config.h>

void HandshakePacket::write(IMCStream *stream)
{
    (void)stream;
    // Does nothing
    throw std::runtime_error("Handshake write should not be called !");
}

void HandshakePacket::read(IMCStream *stream)
{
    protocolVersion = stream->readVarInt();
    serverAddress = stream->readString();
    serverPort = stream->readUnsignedShort();
    nextState = static_cast<ClientState>(stream->readVarInt());
}
