#ifndef __MINESERVER_PACKET_H__
#define __MINESERVER_PACKET_H__

#include <data/stream.h>

class Packet
{
private:
    int id;

    virtual void write(StreamWrapper &wrapper) = 0;

public:
    Packet(int id);
    virtual ~Packet() = default;

    virtual void read(StreamWrapper &wrapper) = 0;
    void send(StreamWrapper &wrapper);
};

#endif // __MINESERVER_PACKET_H__