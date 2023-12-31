#include <gtest/gtest.h>
#include <net/stream.h>
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

void runStreamTest(IMCStream *reader, IMCStream *writer, bool shouldReadVarInt = false)
{
    writer->writeBoolean(true);
    writer->writeByte(SCHAR_MAX);
    writer->writeUnsignedByte(UCHAR_MAX);
    writer->writeShort(SHRT_MAX);
    writer->writeUnsignedShort(SHRT_MAX);
    writer->writeInt(INT_MAX);
    writer->writeFloat(FLT_MAX);
    writer->writeLong(LONG_MAX);
    writer->writeDouble(DBL_MAX);

    std::string s = "Some random st\0ring to io";
    writer->writeString(s);

    writer->writeVarInt(INT_MAX);
    writer->writeVarLong(LONG_MAX);

    writer->flush();

    // Because some buffer-type
    // streams are going to automatically
    // append the size. (eg. ZLibStream)
    if (shouldReadVarInt)
    {
        reader->readVarInt();
    }

    ASSERT_TRUE(reader->readBoolean());
    ASSERT_EQ(reader->readByte(), SCHAR_MAX);
    ASSERT_EQ(reader->readUnsignedByte(), UCHAR_MAX);
    ASSERT_EQ(reader->readShort(), SHRT_MAX);
    ASSERT_EQ(reader->readUnsignedShort(), SHRT_MAX);
    ASSERT_EQ(reader->readInt(), INT_MAX);
    ASSERT_EQ(reader->readFloat(), FLT_MAX);
    ASSERT_EQ(reader->readLong(), LONG_MAX);
    ASSERT_EQ(reader->readDouble(), DBL_MAX);
    ASSERT_EQ(reader->readString(), s);
    ASSERT_EQ(reader->readVarInt(), INT_MAX);
    ASSERT_EQ(reader->readVarLong(), LONG_MAX);
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
    ZLibStream stream0(new MemoryStream(), Z_DEFAULT_COMPRESSION, 0);

    runStreamTest(&stream0, &stream0, true);

    ZLibStream stream10k(new MemoryStream(), Z_DEFAULT_COMPRESSION, 10000);

    runStreamTest(&stream10k, &stream10k, true);
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

TEST(Streams, CryptoZLib)
{
    crypto::ZLibCompressor comp(Z_DEFAULT_COMPRESSION);
    std::string s = "Thiiiiiis iiiiissss some compresssable striiiing !";

    int deLen;
    std::unique_ptr<std::byte[]> de = comp.deflate((std::byte *)s.data(), s.size(), &deLen);
    ASSERT_NE(0, deLen);
    ASSERT_NE(s.size(), deLen);
    ASSERT_NE(s, std::string((char *)de.get(), deLen));

    int inLen = s.size();
    std::unique_ptr<std::byte[]> in = comp.inflate(de.get(), deLen, &inLen);
    ASSERT_NE(0, inLen);
    ASSERT_EQ(inLen, s.size());
    ASSERT_EQ(s, std::string((char *)in.get(), inLen));
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
