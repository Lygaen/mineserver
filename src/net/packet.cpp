/**
 * @file packet.cpp
 * @author Lygaen
 * @brief The file containing the packet logic
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "packet.h"
#include <utils/logger.h>

void IPacket::send(IMCStream *stream)
{
    MemoryStream m;
    m.writeVarInt(id);
    write(&m);

    std::vector<std::byte> d = m.getData();

    stream->finishPacketWrite(&d[0], d.size());

    logger::debug("C<-S Len:%d Id:%d", d.size(), id);
}