#ifndef __MINESERVER_HANDSHAKE_H__
#define __MINESERVER_HANDSHAKE_H__

#include <data/packets/packet.h>

class Handshake : Packet
{
private:
    void write(StreamWrapper &wrapper);

public:
    Handshake() : Packet(0x00) {}
    ~Handshake() = default;

    int protocolVersion;
    std::string serverAddress;
    unsigned short port;
    int nextState;

    void read(StreamWrapper &wrapper);
};

#endif // __MINESERVER_HANDSHAKE_H__