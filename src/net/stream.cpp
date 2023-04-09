#include "stream.h"
#include <cstring>
#include <bit>
#include <stdexcept>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

bool IStream::readBoolean()
{
    std::byte b[sizeof(bool)];
    read(b, 0, sizeof(bool));
    return *reinterpret_cast<bool *>(b);
}

void IStream::writeBoolean(bool b)
{
    write(reinterpret_cast<std::byte *>(&b), 0, sizeof(bool));
}

std::int8_t IStream::readByte()
{
    std::byte b[sizeof(std::int8_t)];
    read(b, 0, sizeof(std::int8_t));
    return *reinterpret_cast<std::int8_t *>(b);
}

void IStream::writeByte(std::int8_t b)
{
    write(reinterpret_cast<std::byte *>(&b), 0, sizeof(std::int8_t));
}

std::uint8_t IStream::readUnsignedByte()
{
    std::int8_t b = readByte();
    return *reinterpret_cast<std::uint8_t *>(&b);
}

void IStream::writeUnsignedByte(std::uint8_t b)
{
    writeByte(*reinterpret_cast<std::int8_t *>(&b));
}

std::int16_t IStream::readShort()
{
    std::byte b[sizeof(std::int16_t)];
    read(b, 0, sizeof(std::int16_t));
    if constexpr (std::endian::little == std::endian::little)
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

void IStream::writeShort(std::int16_t s)
{
    if constexpr (std::endian::little == std::endian::little)
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

std::uint16_t IStream::readUnsignedShort()
{
    std::int16_t b = readShort();
    return *reinterpret_cast<std::uint16_t *>(&b);
}

void IStream::writeUnsignedShort(std::uint16_t s)
{
    writeShort(*reinterpret_cast<std::int16_t *>(&s));
}

std::int32_t IStream::readInt()
{
    std::byte b[sizeof(std::int32_t)];
    read(b, 0, sizeof(std::int32_t));

    if constexpr (std::endian::little == std::endian::little)
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

void IStream::writeInt(std::int32_t i)
{
    if constexpr (std::endian::little == std::endian::little)
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

std::int64_t IStream::readLong()
{
    std::byte b[sizeof(std::int64_t)];
    read(b, 0, sizeof(std::int64_t));

    if constexpr (std::endian::little == std::endian::little)
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

void IStream::writeLong(std::int64_t l)
{
    if constexpr (std::endian::little == std::endian::little)
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

float IStream::readFloat()
{
    std::int32_t i = readInt();
    return *reinterpret_cast<float *>(&i);
}

void IStream::writeFloat(float f)
{
    writeInt(*reinterpret_cast<std::int32_t *>(&f));
}

double IStream::readDouble()
{
    static_assert(sizeof(double) == sizeof(std::int64_t));
    std::int64_t i = readLong();
    return *reinterpret_cast<double *>(&i);
}

void IStream::writeDouble(double d)
{
    writeLong(*reinterpret_cast<std::int64_t *>(&d));
}

std::string IStream::readString()
{
    std::int32_t len = readVarInt();
    std::int8_t b[len];
    read(reinterpret_cast<std::byte *>(b), 0, len);
    return std::string(reinterpret_cast<const char *>(b), len);
}

void IStream::writeString(const std::string &s)
{
    writeVarInt(s.size());
    write(reinterpret_cast<std::byte *>(const_cast<char *>(s.data())), 0, s.size());
}

ChatMessage IStream::readChat()
{
    ChatMessage m;

    std::string json = readString();
    rapidjson::Document doc(rapidjson::kObjectType);
    doc.Parse(json.c_str());

    m.load(doc);

    return m;
}

void IStream::writeChat(const ChatMessage &c)
{
    rapidjson::Document doc(rapidjson::kObjectType);
    c.save(doc, doc.GetAllocator());

    rapidjson::StringBuffer buffer;
    buffer.Clear();

    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    writeString(buffer.GetString());
}

#define SEGMENT_BITS 0x7F
#define CONTINUE_BIT 0x80
std::int32_t IStream::readVarInt()
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

        position += 7;

        if (position > 32)
            throw new std::runtime_error("VarInt is too big !");
    }

    return value;
}

void IStream::writeVarInt(std::int32_t i)
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

std::int64_t IStream::readVarLong()
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
            throw new std::runtime_error("VarLong is too big !");
    }

    return value;
}

void IStream::writeVarLong(std::int64_t l)
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
    data.clear();
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