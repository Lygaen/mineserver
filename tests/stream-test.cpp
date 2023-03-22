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
