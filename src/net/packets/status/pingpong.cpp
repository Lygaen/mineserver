#include "pingpong.h"

void PingPongPacket::write(IStream *stream)
{
    stream->writeLong(payload);
}

void PingPongPacket::read(IStream *stream)
{
    payload = stream->readLong();
}
