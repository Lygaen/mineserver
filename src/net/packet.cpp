#include "packet.h"
#include <utils/logger.h>

void IPacket::send(IMCStream *stream)
{
    MemoryStream m;
    m.writeVarInt(id);
    write(&m);

    std::vector<std::byte> d = m.getData();

    stream->writeVarInt(d.size());

    stream->write(&d[0], 0, d.size());

    logger::debug("C<-S Len:%d Id:%d", d.size(), id);
}