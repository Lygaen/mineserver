#include "pingpong.h"

void PingPong::read(StreamWrapper &wrapper)
{
    payload = wrapper.readLong();
}

void PingPong::write(StreamWrapper &wrapper)
{
    wrapper.writeLong(payload);
}
