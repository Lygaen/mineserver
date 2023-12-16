#include "stream.h"
#include <cstring>
#include <bit>
#include <stdexcept>
#include <algorithm>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

bool IMCStream::readBoolean()
{
    std::byte b[sizeof(bool)];
    read(b, 0, sizeof(bool));
    return *reinterpret_cast<bool *>(b);
}

void IMCStream::writeBoolean(bool b)
{
    write(reinterpret_cast<std::byte *>(&b), 0, sizeof(bool));
}

std::int8_t IMCStream::readByte()
{
    std::byte b[sizeof(std::int8_t)];
    read(b, 0, sizeof(std::int8_t));
    return *reinterpret_cast<std::int8_t *>(b);
}

void IMCStream::writeByte(std::int8_t b)
{
    write(reinterpret_cast<std::byte *>(&b), 0, sizeof(std::int8_t));
}

std::uint8_t IMCStream::readUnsignedByte()
{
    std::int8_t b = readByte();
    return *reinterpret_cast<std::uint8_t *>(&b);
}

void IMCStream::writeUnsignedByte(std::uint8_t b)
{
    writeByte(*reinterpret_cast<std::int8_t *>(&b));
}

std::int16_t IMCStream::readShort()
{
    std::byte b[sizeof(std::int16_t)];
    read(b, 0, sizeof(std::int16_t));
    if constexpr (std::endian::native == std::endian::little)
    {
        std::byte temp; // Yes, I know, It is not needed to use a temp
                        // variable to swap these two bytes but it increases
                        // readability
        temp = b[0];
        b[0] = b[1];
        b[1] = temp;
    }

    return *reinterpret_cast<std::int16_t *>(b);
}

void IMCStream::writeShort(std::int16_t s)
{
    if constexpr (std::endian::native == std::endian::little)
    {
        std::byte *bytes = reinterpret_cast<std::byte *>(&s);
        std::byte temp; // Yes, I know, It is not needed to use a temp
                        // variable to swap these two bytes but it increases
                        // readability
        temp = bytes[0];
        bytes[0] = bytes[1];
        bytes[1] = temp;

        write(bytes, 0, sizeof(std::int16_t));
    }
    else
    {
        write(reinterpret_cast<std::byte *>(&s), 0, sizeof(std::int16_t));
    }
}

std::uint16_t IMCStream::readUnsignedShort()
{
    std::int16_t b = readShort();
    return *reinterpret_cast<std::uint16_t *>(&b);
}

void IMCStream::writeUnsignedShort(std::uint16_t s)
{
    writeShort(*reinterpret_cast<std::int16_t *>(&s));
}

std::int32_t IMCStream::readInt()
{
    std::byte b[sizeof(std::int32_t)];
    read(b, 0, sizeof(std::int32_t));

    if constexpr (std::endian::native == std::endian::little)
    {
        // Kinda tedious but better than nothing !
        std::uint32_t i = *reinterpret_cast<std::uint32_t *>(&b);
        i = __builtin_bswap32(i);
        return *reinterpret_cast<std::int32_t *>(&i);
    }
    else
    {
        return *reinterpret_cast<std::int32_t *>(&b);
    }
}

void IMCStream::writeInt(std::int32_t i)
{
    if constexpr (std::endian::native == std::endian::little)
    {
        std::uint32_t n = *reinterpret_cast<std::uint32_t *>(&i);
        n = __builtin_bswap32(n);
        write(reinterpret_cast<std::byte *>(&n), 0, sizeof(std::int32_t));
    }
    else
    {
        write(reinterpret_cast<std::byte *>(&i), 0, sizeof(std::int32_t));
    }
}

std::int64_t IMCStream::readLong()
{
    std::byte b[sizeof(std::int64_t)];
    read(b, 0, sizeof(std::int64_t));

    if constexpr (std::endian::native == std::endian::little)
    {
        // Again, kinda tedious but better than nothing !
        std::uint64_t i = *reinterpret_cast<std::uint64_t *>(&b);
        i = __builtin_bswap64(i);
        return *reinterpret_cast<std::int64_t *>(&i);
    }
    else
    {
        return *reinterpret_cast<std::int64_t *>(&b);
    }
}

void IMCStream::writeLong(std::int64_t l)
{
    if constexpr (std::endian::native == std::endian::little)
    {
        std::uint64_t n = *reinterpret_cast<std::uint64_t *>(&l);
        n = __builtin_bswap64(n);
        write(reinterpret_cast<std::byte *>(&n), 0, sizeof(std::int64_t));
    }
    else
    {
        write(reinterpret_cast<std::byte *>(&l), 0, sizeof(std::int64_t));
    }
}

float IMCStream::readFloat()
{
    std::int32_t i = readInt();
    return *reinterpret_cast<float *>(&i);
}

void IMCStream::writeFloat(float f)
{
    writeInt(*reinterpret_cast<std::int32_t *>(&f));
}

double IMCStream::readDouble()
{
    static_assert(sizeof(double) == sizeof(std::int64_t));
    std::int64_t i = readLong();
    return *reinterpret_cast<double *>(&i);
}

void IMCStream::writeDouble(double d)
{
    writeLong(*reinterpret_cast<std::int64_t *>(&d));
}

std::string IMCStream::readString()
{
    std::int32_t len = readVarInt();
    std::byte *b = new std::byte[len];
    read(reinterpret_cast<std::byte *>(b), 0, len);
    std::string s = std::string(reinterpret_cast<const char *>(b), len);
    delete[] b;
    return s;
}

void IMCStream::writeString(const std::string &s)
{
    writeVarInt(s.size());
    write(reinterpret_cast<std::byte *>(const_cast<char *>(s.data())), 0, s.size());
}

ChatMessage IMCStream::readChat()
{
    ChatMessage m;

    std::string json = readString();
    rapidjson::Document doc(rapidjson::kObjectType);
    doc.Parse(json.c_str());

    m.load(doc);

    return m;
}

void IMCStream::writeChat(const ChatMessage &c)
{
    rapidjson::Document doc(rapidjson::kObjectType);
    c.save(doc, doc.GetAllocator());

    rapidjson::StringBuffer buffer;
    buffer.Clear();

    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    writeString(buffer.GetString());
}

/**
 * @brief Mask for VarInts
 *
 * Mask for bit representation of varints,
 * the most bit used to continue or not
 */
#define SEGMENT_BITS 0x7F
/**
 * @brief Mask for Continue Bit
 *
 * Mask for the bit representation of the
 * continue bit, used to see if we need
 * to load the next byte in
 */
#define CONTINUE_BIT 0x80
std::int32_t IMCStream::readVarInt()
{
    std::int32_t value = 0;
    int position = 0;
    char currentByte;

    while (true)
    {
        currentByte = readByte();
        value |= (currentByte & SEGMENT_BITS) << position;

        if ((currentByte & CONTINUE_BIT) == 0)
            break;

        // Forward to the next 7 bits
        position += 7;

        if (position > 32)
            throw std::runtime_error("VarInt is too big !");
    }

    return value;
}

void IMCStream::writeVarInt(std::int32_t i)
{
    while (true)
    {
        if ((i & ~SEGMENT_BITS) == 0)
        {
            writeByte(i);
            return;
        }

        writeByte((i & SEGMENT_BITS) | CONTINUE_BIT);

        i >>= 7;
    }
}

std::int64_t IMCStream::readVarLong()
{
    std::int64_t value = 0;
    int position = 0;
    std::int8_t currentByte;

    while (true)
    {
        currentByte = readByte();
        value |= (long)(currentByte & SEGMENT_BITS) << position;

        if ((currentByte & CONTINUE_BIT) == 0)
            break;

        position += 7;

        if (position > 64)
            throw std::runtime_error("VarLong is too big !");
    }

    return value;
}

void IMCStream::writeVarLong(std::int64_t l)
{
    while (true)
    {
        if ((l & ~((long)SEGMENT_BITS)) == 0)
        {
            writeByte(l);
            return;
        }

        writeByte((l & SEGMENT_BITS) | CONTINUE_BIT);

        l >>= 7;
    }
}

void IMCStream::writeUUID(const UUID &uuid)
{
    const std::byte *buff = uuid.getBytes();

    write(const_cast<std::byte *>(buff), 0, 16);
}

UUID IMCStream::readUUID()
{
    std::byte *buff = new std::byte[16];
    read(buff, 0, 16);
    UUID uuid = UUID::fromBytes(buff);
    delete[] buff;

    return uuid;
}

void MemoryStream::read(std::byte *buffer, std::size_t offset, std::size_t len)
{
    std::memcpy(buffer + offset, data.data() + readIndex, std::min(len, data.size()));
    readIndex += std::min(len, data.size());
}

void MemoryStream::write(std::byte *buffer, std::size_t offset, std::size_t len)
{
    std::copy(buffer + offset, buffer + offset + len, std::back_inserter(data));
}

void MemoryStream::flush()
{
    readIndex = 0;
}

void MemoryStream::clear()
{
    flush();
    data.clear();
}

const std::vector<std::byte> &MemoryStream::getData() const
{
    return data;
}

NetSocketStream::NetSocketStream(ClientSocket socket) : socket(socket)
{
}

NetSocketStream::~NetSocketStream()
{
    socket.close();
}

void NetSocketStream::read(std::byte *buffer, std::size_t offset, std::size_t len)
{
    socket.read(buffer + offset, len);
}

void NetSocketStream::write(std::byte *buffer, std::size_t offset, std::size_t len)
{
    socket.write(buffer + offset, len);
}

void NetSocketStream::flush()
{
    /* Not implemented */
}

CipherStream::CipherStream(IMCStream *baseStream, std::byte *key, std::byte *iv) : baseStream(baseStream),
                                                                                   encipher(crypto::CipherState::ENCRYPT, key, iv),
                                                                                   decipher(crypto::CipherState::DECRYPT, key, iv)

{
}

CipherStream::~CipherStream()
{
    delete baseStream;
}

void CipherStream::read(std::byte *buffer, std::size_t offset, std::size_t len)
{
    std::byte *buf = new std::byte[len];
    baseStream->read(buf, 0, len);

    std::byte *outBuf = new std::byte[decipher.calculateBufferSize(len)];
    int outLen = decipher.update(buf, len, outBuf);
    delete[] buf;

    std::memcpy(buffer + offset, outBuf, outLen);
    delete[] outBuf;
}

void CipherStream::write(std::byte *buffer, std::size_t offset, std::size_t len)
{
    std::byte *outBuf = new std::byte[encipher.calculateBufferSize(len)];
    int outLen = encipher.update(buffer + offset, len, outBuf);

    baseStream->write(outBuf, 0, outLen);
    delete[] outBuf;
}

void CipherStream::flush()
{
    baseStream->flush();
}

ZLibStream::ZLibStream(IMCStream *baseStream, int level) : baseStream(baseStream), comp(level)
{
}

ZLibStream::~ZLibStream()
{
    delete baseStream;
}

void ZLibStream::read(std::byte *buffer, std::size_t offset, std::size_t len)
{
    std::memcpy(buffer + offset, inBuffer.data() + inIndex, std::min(len, inBuffer.size()));
    inIndex += std::min(len, inBuffer.size());
}

void ZLibStream::write(std::byte *buffer, std::size_t offset, std::size_t len)
{
    std::copy(buffer + offset, buffer + offset + len, std::back_inserter(outBuffer));
}

void ZLibStream::flush()
{

    // Buffer write
    if (outBuffer.size() > 0)
    {
        int dataLength = outBuffer.size();
        int packetLength = 0;
        std::unique_ptr<std::byte[]> inData = comp.deflate(outBuffer.data(), dataLength, &packetLength);

        MemoryStream m;
        m.writeVarInt(dataLength); // Adds dataLength to the buffer because it is used
        packetLength += m.getData().size();

        baseStream->writeVarInt(packetLength);
        baseStream->writeVarInt(dataLength);
        baseStream->write(inData.get(), 0, packetLength - m.getData().size());
        outBuffer.clear();
    }

    // Buffer read
    inBuffer.clear();

    int packetLength = baseStream->readVarInt();
    inBuffer.reserve(packetLength);

    int dataLength = baseStream->readVarInt();

    MemoryStream m;
    m.writeVarInt(dataLength); // Adds dataLength to the buffer because it is used
    std::copy(m.getData().begin(), m.getData().end(), std::back_inserter(inBuffer));

    std::byte *streamIn = new std::byte[packetLength - inBuffer.size()];
    baseStream->read(streamIn, 0, packetLength - inBuffer.size());
    int outLen = dataLength;
    std::unique_ptr<std::byte[]> data = comp.inflate(streamIn, packetLength - inBuffer.size(), &dataLength);
    delete[] streamIn;

    std::copy(data.get(), data.get() + outLen, std::back_inserter(inBuffer));

    baseStream->flush();
}
