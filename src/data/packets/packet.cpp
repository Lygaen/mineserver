#include "packet.h"

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

    char *data = s.data();
    int size = s.size();
    wrapper.writeVarInt(size);
    wrapper.writeBytes(data, size);
}
