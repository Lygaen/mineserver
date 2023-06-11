#include "packet.h"

void IPacket::send(IStream *stream)
{
    MemoryStream m;
    m.writeVarInt(id);
    write(&m);

    std::vector<std::byte> d = m.getData();

    stream->writeVarInt(d.size());

    stream->write(&d[0], 0, d.size());
}