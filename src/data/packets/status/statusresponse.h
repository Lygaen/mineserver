#ifndef __MINESERVER_STATUSRESPONSE_H__
#define __MINESERVER_STATUSRESPONSE_H__

#include <data/packets/packet.h>

class StatusResponse : public Packet
{
private:
    void write(StreamWrapper &wrapper);

public:
    StatusResponse() : Packet(0x00) {}
    ~StatusResponse() = default;

    void read(StreamWrapper &wrapper);
};

#endif // __MINESERVER_STATUSRESPONSE_H__