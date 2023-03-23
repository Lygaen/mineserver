#include <gtest/gtest.h>
#include <net/stream.h>
#include <limits>

/**
 * @brief Test Loops number
 *
 * The number of loops that the tests must
 * follow with random values to pass.
 */
#define TEST_LOOP 10

TEST(MinecraftStream, Boolean)
{
    MemoryStream m;

    m.writeBoolean(true);

    ASSERT_TRUE(m.readBoolean());

    m.flush();
    m.writeBoolean(false);
    ASSERT_FALSE(m.readBoolean());
}

TEST(MinecraftStream, Byte)
{
    MemoryStream m;

    for (size_t i = 0; i < TEST_LOOP; i++)
    {
        std::int8_t z = SCHAR_MIN + rand() % (SCHAR_MAX - SCHAR_MIN);
        m.writeByte(z);

        ASSERT_EQ(m.readByte(), z);
        m.flush();
    }

    // So that we test the full 1's possibilty
    m.writeByte(SCHAR_MAX);
    ASSERT_EQ(m.readByte(), SCHAR_MAX);
}

TEST(MinecraftStream, UnsignedByte)
{
    MemoryStream m;

    for (size_t i = 0; i < TEST_LOOP; i++)
    {
        std::uint8_t z = 0 + rand() % (0 - CHAR_MIN);
        m.writeUnsignedByte(z);

        ASSERT_EQ(m.readUnsignedByte(), z);
        m.flush();
    }

    // So that we test the full 1's possibilty
    m.writeUnsignedByte(UCHAR_MAX);
    ASSERT_EQ(m.readUnsignedByte(), UCHAR_MAX);
}

TEST(MinecraftStream, Short)
{
    MemoryStream m;

    for (size_t i = 0; i < TEST_LOOP; i++)
    {
        std::int16_t z = SHRT_MIN + rand() % (SHRT_MIN - SHRT_MAX);
        m.writeShort(z);

        ASSERT_EQ(m.readShort(), z);
        m.flush();
    }

    // So that we test the full 1's possibilty
    m.writeShort(SHRT_MAX);
    ASSERT_EQ(m.readShort(), SHRT_MAX);
}

TEST(MinecraftStream, UnsignedShort)
{
    MemoryStream m;

    for (size_t i = 0; i < TEST_LOOP; i++)
    {
        std::uint16_t z = 0 + rand() % (0 - USHRT_MAX);
        m.writeUnsignedShort(z);

        ASSERT_EQ(m.readUnsignedShort(), z);
        m.flush();
    }

    // So that we test the full 1's possibilty
    m.writeUnsignedShort(SHRT_MAX);
    ASSERT_EQ(m.readUnsignedShort(), SHRT_MAX);
}

TEST(MinecraftStream, Int)
{
    MemoryStream m;

    for (size_t i = 0; i < TEST_LOOP; i++)
    {
        std::int32_t z = rand();
        m.writeInt(z);

        ASSERT_EQ(m.readInt(), z);
        m.flush();
    }

    // So that we test the full 1's possibilty
    m.writeInt(INT_MAX);
    ASSERT_EQ(m.readInt(), INT_MAX);
}

TEST(MinecraftStream, Long)
{
    MemoryStream m;

    for (size_t i = 0; i < TEST_LOOP; i++)
    {
        std::int64_t z = rand();
        m.writeLong(z);

        ASSERT_EQ(m.readLong(), z);
        m.flush();
    }

    // So that we test the full 1's possibilty
    m.writeLong(LONG_MAX);
    ASSERT_EQ(m.readLong(), LONG_MAX);
}

TEST(MinecraftStream, Float)
{
    MemoryStream m;

    for (size_t i = 0; i < TEST_LOOP; i++)
    {
        std::int32_t z = rand();
        m.writeFloat(*reinterpret_cast<float *>(&z));

        ASSERT_EQ(m.readFloat(), *reinterpret_cast<float *>(&z));
        m.flush();
    }
}

TEST(MinecraftStream, Double)
{
    MemoryStream m;

    for (size_t i = 0; i < TEST_LOOP; i++)
    {
        std::int32_t z = rand();
        m.writeDouble(*reinterpret_cast<double *>(&z));

        ASSERT_EQ(m.readDouble(), *reinterpret_cast<double *>(&z));
        m.flush();
    }
}

TEST(MinecraftStream, String)
{
    MemoryStream m;

    std::string s = "Some random st\0ring to io";
    m.writeString(s);

    ASSERT_EQ(m.readString(), s);
}

TEST(MinecraftStream, VarInt)
{
    MemoryStream m;

    for (size_t i = 0; i < TEST_LOOP; i++)
    {
        std::int32_t z = rand();
        m.writeVarInt(z);

        ASSERT_EQ(m.readVarInt(), z);
        m.flush();
    }

    // So that we test the full 1's possibilty
    m.writeVarInt(INT_MAX);
    ASSERT_EQ(m.readVarInt(), INT_MAX);
}

TEST(MinecraftStream, VarLong)
{
    MemoryStream m;

    for (size_t i = 0; i < TEST_LOOP; i++)
    {
        std::int64_t z = rand();
        m.writeVarLong(z);

        ASSERT_EQ(m.readVarLong(), z);
        m.flush();
    }

    // So that we test the full 1's possibilty
    m.writeVarLong(LONG_MAX);
    ASSERT_EQ(m.readVarLong(), LONG_MAX);
}

void runStreamTest(IStream *reader, IStream *writer)
{
    writer->writeBoolean(true);
    ASSERT_TRUE(reader->readBoolean());

    writer->writeByte(SCHAR_MAX);
    ASSERT_EQ(reader->readByte(), SCHAR_MAX);

    writer->writeUnsignedByte(UCHAR_MAX);
    ASSERT_EQ(reader->readUnsignedByte(), UCHAR_MAX);

    writer->writeShort(SHRT_MAX);
    ASSERT_EQ(reader->readShort(), SHRT_MAX);

    writer->writeUnsignedShort(SHRT_MAX);
    ASSERT_EQ(reader->readUnsignedShort(), SHRT_MAX);

    writer->writeInt(INT_MAX);
    ASSERT_EQ(reader->readInt(), INT_MAX);

    writer->writeFloat(FLT_MAX);
    ASSERT_EQ(reader->readFloat(), FLT_MAX);

    writer->writeLong(LONG_MAX);
    ASSERT_EQ(reader->readLong(), LONG_MAX);

    writer->writeDouble(DBL_MAX);
    ASSERT_EQ(reader->readDouble(), DBL_MAX);

    std::string s = "Some random st\0ring to io";
    writer->writeString(s);
    ASSERT_EQ(reader->readString(), s);

    writer->writeVarInt(INT_MAX);
    ASSERT_EQ(reader->readVarInt(), INT_MAX);

    writer->writeVarLong(LONG_MAX);
    ASSERT_EQ(reader->readVarLong(), LONG_MAX);
}

TEST(Streams, Network)
{
    ASSERT_TRUE(ServerSocket::init());

    ServerSocket server(SOCK_STREAM);
    server.bind(nullptr, 6565);
    server.start(10);

    ClientSocket client(SOCK_STREAM);
    bool hasConnected;
    std::thread([&client, &hasConnected]()
                { hasConnected = client.connect("127.0.0.1", 6565); })
        .detach();
    ClientSocket sClient = server.accept();
    ASSERT_TRUE(hasConnected);

    NetSocketStream clientStream(client);
    NetSocketStream serverStream(sClient);

    runStreamTest(&clientStream, &serverStream);
    runStreamTest(&serverStream, &clientStream);

    server.close();
    client.close();
    ASSERT_TRUE(ServerSocket::cleanup());
}