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
