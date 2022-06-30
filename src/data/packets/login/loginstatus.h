#ifndef __MINESERVER_LOGINSTATUS_H__
#define __MINESERVER_LOGINSTATUS_H__

#include <data/packets/packet.h>

class LoginStart : public Packet
{
private:
    void write(StreamWrapper &wrapper);

public:
    LoginStart() : Packet(0x00) {}
    ~LoginStart() = default;

    std::string playerName;

    void read(StreamWrapper &wrapper);
};

class LoginSuccess : public Packet
{
private:
    void write(StreamWrapper &wrapper);
    std::string uuid;
    std::string username;

public:
    LoginSuccess(const std::string &uuid, const std::string &username) : Packet(0x00), uuid(uuid), username(username) {}
    ~LoginSuccess() = default;

    void read(StreamWrapper &wrapper);
};

#endif // __MINESERVER_LOGINSTATUS_H__