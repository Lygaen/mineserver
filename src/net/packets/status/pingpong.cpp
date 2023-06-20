#include "pingpong.h"

void PingPongPacket::write(IMCStream *stream)
{
    stream->writeLong(payload);
}

void PingPongPacket::read(IMCStream *stream)
{
    payload = stream->readLong();
}
