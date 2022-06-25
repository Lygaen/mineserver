#include "stream.h"
#include <cpnet-network.h>
#include <cstring>

SocketStream::SocketStream(socket_t s) : sock(s)
{
}

char *SocketStream::read(int len)
{
    char *ptr = new char[len];
    cpnet_read(sock, ptr, len);
    return ptr;
}

void SocketStream::write(char *data, int len)
{
    cpnet_write(sock, data, len);
}

StreamWrapper::StreamWrapper(Stream &s) : stream(s)
{
}

char StreamWrapper::readByte()
{
    return inBuffer.at(inIndex++);
}

void StreamWrapper::writeByte(char c)
{
    outBuffer.push_back(c);
}

unsigned char StreamWrapper::readUnsignedByte()
{
    return *reinterpret_cast<unsigned char *>(&inBuffer.at(inIndex++));
}

void StreamWrapper::writeUnsignedByte(unsigned char c)
{
    outBuffer.push_back(*reinterpret_cast<char *>(&c));
}

bool StreamWrapper::readBool()
{
    return readByte();
}

void StreamWrapper::writeBool(bool b)
{
    outBuffer.push_back(b);
}

short StreamWrapper::readShort()
{
    unsigned short s = readUnsignedShort();
    return *reinterpret_cast<short *>(&s);
}

void StreamWrapper::writeShort(short s)
{
    writeUnsignedShort(*reinterpret_cast<unsigned short *>(&s));
}

unsigned short StreamWrapper::readUnsignedShort()
{
    unsigned short s = *reinterpret_cast<unsigned short *>(inBuffer.data() + inIndex);

#if __LITTLE_ENDIAN__
    s = __builtin_bswap16(s);
#endif

    inIndex += sizeof(unsigned short);
    return s;
}

void StreamWrapper::writeUnsignedShort(unsigned short s)
{
#if __LITTLE_ENDIAN__
    s = __builtin_bswap16(s);
#endif

    char *c = reinterpret_cast<char *>(&s);
    outBuffer.insert(outBuffer.end(), {c[0], c[1]});
}

int StreamWrapper::readInt()
{
    unsigned int i = *reinterpret_cast<unsigned int *>(inBuffer.data() + inIndex);

#if __LITTLE_ENDIAN__
    i = __builtin_bswap32(i);
#endif

    inIndex += sizeof(unsigned int);
    return *reinterpret_cast<int *>(&i);
}

void StreamWrapper::writeInt(int i)
{
#if __LITTLE_ENDIAN__
    i = __builtin_bswap32(i);
#endif

    char *c = reinterpret_cast<char *>(&i);
    outBuffer.insert(outBuffer.end(), {c[0], c[1], c[2], c[3]});
}

long long StreamWrapper::readLong()
{
    unsigned long long l = *reinterpret_cast<unsigned long long *>(inBuffer.data() + inIndex);

#if __LITTLE_ENDIAN__
    l = __builtin_bswap64(l);
#endif

    inIndex += sizeof(unsigned long long);
    return *reinterpret_cast<long *>(&l);
}

void StreamWrapper::writeLong(long long l)
{
#if __LITTLE_ENDIAN__
    l = __builtin_bswap64(l);
#endif

    char *c = reinterpret_cast<char *>(&l);
    outBuffer.insert(outBuffer.end(), {c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]});
}

float StreamWrapper::readFloat()
{
    int i = readInt();
    return *reinterpret_cast<float *>(&i);
}

void StreamWrapper::writeFloat(float f)
{
    writeInt(*reinterpret_cast<int *>(&f));
}

double StreamWrapper::readDouble()
{
    long l = readLong();
    return *reinterpret_cast<double *>(&l);
}

void StreamWrapper::writeDouble(double d)
{
    writeLong(*reinterpret_cast<long *>(&d));
}

void StreamWrapper::startRead(int len)
{
    char *ptr = stream.read(len);
    inBuffer = std::vector(ptr, ptr + len);
    inIndex = 0;
}

void StreamWrapper::flush()
{
    stream.write(outBuffer.data(), outBuffer.size());
    outBuffer.clear();
}
