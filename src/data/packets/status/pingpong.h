#ifndef __MINESERVER_PINGPONG_H__
#define __MINESERVER_PINGPONG_H__

#include <data/packets/packet.h>

class PingPong : public Packet
{
private:
    long long payload;

    void write(StreamWrapper &wrapper);

public:
    PingPong() : Packet(0x01) {}
    ~PingPong() = default;

    void read(StreamWrapper &wrapper);
};

#endif // __MINESERVER_PINGPONG_H__