#include "handshake.h"

void Handshake::write(StreamWrapper &wrapper)
{
}

void Handshake::read(StreamWrapper &wrapper)
{
    protocolVersion = wrapper.readVarInt();
    wrapper.readString(serverAddress);
    port = wrapper.readUnsignedShort();
    nextState = wrapper.readVarInt();
}
