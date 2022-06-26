#ifndef __MINESERVER_STREAM_H__
#define __MINESERVER_STREAM_H__

#include <cpnet-network.h>
#include <vector>
#include <string>

class Stream
{
public:
    virtual char *read(int len) = 0;
    virtual void write(char *data, int len) = 0;
};

class SocketStream : public Stream
{
private:
    socket_t sock;

public:
    SocketStream(socket_t s);
    ~SocketStream() = default;

    char *read(int len);
    void write(char *data, int len);
};

class StreamWrapper
{
private:
    Stream &stream;
    int inIndex;
    std::vector<char> inBuffer;
    std::vector<char> outBuffer;

public:
    StreamWrapper(Stream &s);
    ~StreamWrapper() = default;

    char readByte();
    void writeByte(char c);

    unsigned char readUnsignedByte();
    void writeUnsignedByte(unsigned char c);

    bool readBool();
    void writeBool(bool b);

    short readShort();
    void writeShort(short s);

    unsigned short readUnsignedShort();
    void writeUnsignedShort(unsigned short s);

    int readInt();
    void writeInt(int i);

    long long readLong();
    void writeLong(long long l);

    float readFloat();
    void writeFloat(float f);

    double readDouble();
    void writeDouble(double d);

    int readVarInt();
    void writeVarInt(int i);

    long long readVarLong();
    void writeVarLong(long long l);

    void readString(std::string &str);
    void writeString(const std::string &str);

    void writeBytes(char *c, int len);
    char *readBytes(int len);

    void startRead(int len);
    void flush();
};

#endif // __MINESERVER_STREAM_H__