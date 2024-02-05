#include <gtest/gtest.h>
#include <net/stream.h>
#include <net/packet.h>
#include <utils/crypto.h>
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

    m.clear();
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
        m.clear();
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
        m.clear();
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
        m.clear();
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
        m.clear();
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
        m.clear();
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
        m.clear();
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
        m.clear();
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
        m.clear();
    }
}

TEST(MinecraftStream, String)
{
    MemoryStream m;

    std::string s = "Some random st\0ring to io";
    m.writeString(s);

    ASSERT_EQ(m.readString(), s);
}

TEST(MinecraftStream, Chat)
{
    MemoryStream m;

    ChatMessage c = std::string("Some random st\0ring to io");
    m.writeChat(c);

    ASSERT_EQ(m.readChat(), c);
}

TEST(MinecraftStream, VarInt)
{
    MemoryStream m;

    for (size_t i = 0; i < TEST_LOOP; i++)
    {
        std::int32_t z = rand();
        m.writeVarInt(z);

        ASSERT_EQ(m.readVarInt(), z);
        m.clear();
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
        m.clear();
    }

    // So that we test the full 1's possibilty
    m.writeVarLong(LONG_MAX);
    ASSERT_EQ(m.readVarLong(), LONG_MAX);
}

class TestPacket : public IPacket
{
private:
    std::string s = "Some random st\0ring to io";

protected:
    void write(IMCStream *stream) override
    {
        stream->writeBoolean(true);
        stream->writeByte(SCHAR_MAX);
        stream->writeUnsignedByte(UCHAR_MAX);
        stream->writeShort(SHRT_MAX);
        stream->writeUnsignedShort(SHRT_MAX);
        stream->writeInt(INT_MAX);
        stream->writeFloat(FLT_MAX);
        stream->writeLong(LONG_MAX);
        stream->writeDouble(DBL_MAX);

        stream->writeString(s);

        stream->writeVarInt(INT_MAX);
        stream->writeVarLong(LONG_MAX);
    }

public:
    TestPacket() : IPacket(0x03) {}
    ~TestPacket() override = default;

    void read(IMCStream *stream) override
    {
        ASSERT_TRUE(stream->readBoolean());
        ASSERT_EQ(stream->readByte(), SCHAR_MAX);
        ASSERT_EQ(stream->readUnsignedByte(), UCHAR_MAX);
        ASSERT_EQ(stream->readShort(), SHRT_MAX);
        ASSERT_EQ(stream->readUnsignedShort(), SHRT_MAX);
        ASSERT_EQ(stream->readInt(), INT_MAX);
        ASSERT_EQ(stream->readFloat(), FLT_MAX);
        ASSERT_EQ(stream->readLong(), LONG_MAX);
        ASSERT_EQ(stream->readDouble(), DBL_MAX);

        ASSERT_EQ(stream->readString(), s);
        ASSERT_EQ(stream->readVarInt(), INT_MAX);
        ASSERT_EQ(stream->readVarLong(), LONG_MAX);
    }
};

void runStreamTest(IMCStream *reader, IMCStream *writer)
{
    TestPacket p;
    p.send(writer);
    writer->flush();

    reader->readVarInt();
    int id = reader->readVarInt();
    ASSERT_EQ(id, p.id);

    p.read(reader);
}

#ifndef GITHUB_ACTIONS_BUILD
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
#endif

TEST(Streams, Cipher)
{
    std::unique_ptr<std::byte[]> key = crypto::randomSecure(16);

    // Because Minecraft uses the same bytes for key and iv.
    CipherStream stream(new MemoryStream(), key.get(), key.get());

    runStreamTest(&stream, &stream);
}

TEST(Streams, ZLib)
{
    ZLibStream stream0(new MemoryStream(), 5, 0);

    runStreamTest(&stream0, &stream0);

    ZLibStream stream10k(new MemoryStream(), 5, 10000);

    runStreamTest(&stream10k, &stream10k);
}

TEST(Streams, CryptoRSA)
{
    std::string data = "This is a test !";

    ASSERT_TRUE(crypto::init());

    size_t encryptedLen;
    std::unique_ptr<std::byte[]> encrypted = crypto::rsaEncrypt((std::byte *)data.c_str(), data.length(), &encryptedLen);

    ASSERT_NE(data, std::string((const char *)encrypted.get(), encryptedLen));

    size_t decryptedLen;
    std::unique_ptr<std::byte[]> decrypted = crypto::rsaDecrypt(encrypted.get(), encryptedLen, &decryptedLen);

    ASSERT_EQ(data, std::string((const char *)decrypted.get(), decryptedLen));
    crypto::cleanup();
}

TEST(Streams, CryptoHash)
{
    crypto::MinecraftHash hasher;

    hasher.update("Notch");
    ASSERT_EQ(hasher.finalize(), "4ed1f46bbe04bc756bcb17c0c7ce3e4632f06a48");

    hasher.update("Notch");
    ASSERT_EQ(hasher.finalize(), "4ed1f46bbe04bc756bcb17c0c7ce3e4632f06a48");

    hasher.update("jeb_");
    ASSERT_EQ(hasher.finalize(), "-7c9d5b0044c130109a5d7b5fb5c317c02b4e28c1");

    hasher.update("simon");
    ASSERT_EQ(hasher.finalize(), "88e16a1019277b15d58faf0541e11910eb756f6");
}

TEST(Streams, CryptoCipher)
{
    std::unique_ptr<std::byte[]> d = crypto::randomSecure(16);
    crypto::AES128CFB8Cipher encipher(crypto::CipherState::ENCRYPT, d.get(), d.get());
    crypto::AES128CFB8Cipher decipher(crypto::CipherState::DECRYPT, d.get(), d.get());

    std::string s = "This is some really long data ! It should be allright to encrypt it and get it back.";

    int temp;
    std::byte *enData = new std::byte[encipher.calculateBufferSize(s.size())];
    int enLen = encipher.update((std::byte *)s.c_str(), s.size(), enData);
    enLen += encipher.finalize(enData + enLen);

    ASSERT_NE(s, std::string((char *)enData, enLen));

    std::byte *deData = new std::byte[decipher.calculateBufferSize(s.size())];
    int deLen = decipher.update(enData, enLen, deData);
    deLen += decipher.finalize(deData + deLen);

    ASSERT_EQ(s, std::string((char *)deData, deLen));

    delete[] enData;
    delete[] deData;
}

TEST(Streams, MD5)
{
    std::string s1 = "Et l’unique cordeau des trompettes marines";
    std::string s2 = "Et l’unique cordeau des trompettes marinEs";
    std::string precomputedDigest1 = "8747e564eb53cb2f1dcb9aae0779c2aa";
    std::string precomputedDigest2 = "c802e1bd9b5f2b0d244bbc982f5082b3";

    std::string hashed1 = crypto::md5Digest(s1);

    ASSERT_EQ(precomputedDigest1, hashed1);

    std::string hashed2 = crypto::md5Digest(s2);
    ASSERT_NE(hashed1, hashed2);
    ASSERT_EQ(precomputedDigest2, hashed2);
}
