#include "loginstatus.h"

void LoginStart::read(StreamWrapper &wrapper)
{
    wrapper.readString(playerName);
}

void LoginStart::write(StreamWrapper &wrapper)
{
}

void LoginSuccess::read(StreamWrapper &wrapper)
{
}

void LoginSuccess::write(StreamWrapper &wrapper)
{
    wrapper.writeString(uuid);
    wrapper.writeString(username);
}
