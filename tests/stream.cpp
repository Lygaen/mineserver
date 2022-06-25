#include <gtest/gtest.h>
#include <cpnet-network.h>
#include <data/stream.h>

class BufferStream : public Stream
{
private:
    std::vector<char> buff;

public:
    BufferStream() = default;
    ~BufferStream() = default;

    char *read(int len)
    {
        return buff.data();
    }

    void write(char *data, int len)
    {
        buff = std::vector(data, data + len);
    }
};

TEST(StreamTest, ByteIO)
{
    BufferStream b;
    StreamWrapper w(b);

    char c = 'L';
    w.writeByte(c);
    w.flush();
    w.startRead(sizeof(char));
    EXPECT_EQ(c, w.readByte());
}

TEST(StreamTest, UByteIO)
{
    BufferStream b;
    StreamWrapper w(b);

    unsigned char c = 'L';
    w.writeUnsignedByte(c);
    w.flush();
    w.startRead(sizeof(char));
    EXPECT_EQ(c, w.readUnsignedByte());
}

TEST(StreamTest, BoolIO)
{
    BufferStream b;
    StreamWrapper w(b);

    bool c = true;
    w.writeBool(c);
    w.flush();
    w.startRead(sizeof(char));
    EXPECT_EQ(c, w.readBool());
}
