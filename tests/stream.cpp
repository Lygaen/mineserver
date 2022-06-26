#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <data/stream.h>
#include <doctest/doctest.h>

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

TEST_CASE("Byte IO working")
{
    BufferStream b;
    StreamWrapper w(b);

    char c = 'L';
    w.writeByte(c);
    w.flush();
    w.startRead(sizeof(char));
    CHECK(c == w.readByte());
}

TEST_CASE("UByte IO working")
{
    BufferStream b;
    StreamWrapper w(b);

    unsigned char c = 'L';
    w.writeUnsignedByte(c);
    w.flush();
    w.startRead(sizeof(char));
    CHECK(c == w.readUnsignedByte());
}

TEST_CASE("Bool IO working")
{
    BufferStream b;
    StreamWrapper w(b);

    bool c = true;
    w.writeBool(c);
    w.flush();
    w.startRead(sizeof(char));
    CHECK(c == w.readBool());
}

TEST_CASE("Short IO working")
{
    BufferStream b;
    StreamWrapper w(b);

    short c = -52;
    w.writeShort(c);
    w.flush();
    w.startRead(sizeof(short));
    CHECK(c == w.readShort());
}

TEST_CASE("UShort IO working")
{
    BufferStream b;
    StreamWrapper w(b);

    unsigned short c = 52;
    w.writeUnsignedShort(c);
    w.flush();
    w.startRead(sizeof(unsigned short));
    CHECK(c == w.readUnsignedShort());
}

TEST_CASE("Int IO working")
{
    BufferStream b;
    StreamWrapper w(b);

    int c = -160632;
    w.writeInt(c);
    w.flush();
    w.startRead(sizeof(int));
    CHECK(c == w.readInt());
}

TEST_CASE("Long IO working")
{
    BufferStream b;
    StreamWrapper w(b);

    long long c = -16069456132;
    w.writeLong(c);
    w.flush();
    w.startRead(sizeof(long long));
    CHECK(c == w.readLong());
}

TEST_CASE("Float IO working")
{
    BufferStream b;
    StreamWrapper w(b);

    float c = -160632.156f;
    w.writeFloat(c);
    w.flush();
    w.startRead(sizeof(float));
    CHECK(c == w.readFloat());
}

TEST_CASE("Double IO working")
{
    BufferStream b;
    StreamWrapper w(b);

    double c = -1606516632.156;
    w.writeDouble(c);
    w.flush();
    w.startRead(sizeof(double));
    CHECK(c == w.readDouble());
}

TEST_CASE("VarInt IO working")
{
    BufferStream b;
    StreamWrapper w(b);

    int c = -160632;
    w.writeVarInt(c);
    w.flush();
    w.startRead(sizeof(int) + 1); // allocates the size + 1 (because of varint nature)
    CHECK(c == w.readVarInt());
}

TEST_CASE("VarLong IO working")
{
    BufferStream b;
    StreamWrapper w(b);

    long long c = -16069456132;
    w.writeVarLong(c);
    w.flush();
    w.startRead(sizeof(long long) + 2); // allocates the size + 2 (because of varlong nature)
    CHECK(c == w.readVarLong());
}

TEST_CASE("String IO working")
{
    BufferStream b;
    StreamWrapper w(b);

    std::string c = "Some kind of long string";
    w.writeString(c);
    w.flush();
    w.startRead(sizeof(int) + c.size()); // VarInt + actual string
    std::string temp;
    w.readString(temp);
    CHECK(c == temp);
}

TEST_CASE("Size checking")
{
    // Size matches Java's size
    CHECK(sizeof(short) == 2);
    CHECK(sizeof(int) == 4);
    CHECK(sizeof(long long) == 8);

    // Size matches unsigned equivalent
    CHECK(sizeof(short) == sizeof(unsigned short));
    CHECK(sizeof(int) == sizeof(unsigned int));
    CHECK(sizeof(long long) == sizeof(unsigned long long));

    // Size matches floating equivalent
    CHECK(sizeof(int) == sizeof(float));
    CHECK(sizeof(long long) == sizeof(double));
}