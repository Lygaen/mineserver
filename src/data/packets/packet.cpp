#include "packet.h"
#include <utils/logger.h>

class BufferedWriteStream : public Stream
{
private:
    std::vector<char> buffer;

public:
    char *read(int len)
    {
        return nullptr;
    }

    void write(char *data, int len)
    {
        std::copy(data, data + len, std::back_inserter(buffer));
    }

    char *data()
    {
        return buffer.data();
    }

    int size()
    {
        return buffer.size();
    }
};

Packet::Packet(int id) : id(id)
{
}

void Packet::send(StreamWrapper &wrapper)
{
    BufferedWriteStream s;
    StreamWrapper tempWrapper(s);

    tempWrapper.writeVarInt(id);
    write(tempWrapper);
    tempWrapper.flush();

    char *data = s.data();
    int size = s.size();
    wrapper.writeVarInt(size);
    wrapper.writeBytes(data, size);

    logger::packet(logger::PacketDirection::TO_CLIENT, size, id, "NULL");
}
