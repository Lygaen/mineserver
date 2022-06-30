#ifndef __MINESERVER_PLAYER_H__
#define __MINESERVER_PLAYER_H__

#include <entities/interfaces/livingentity.h>
#include <string>

class Player : public LivingEntity
{
private:
    std::string uuid;
    std::string username;

public:
    Player(const std::string &uuid, const std::string &username) : uuid(uuid), username(username) {}
    ~Player();

    const std::string &getUUID() const { return uuid; }
    const std::string &getUsername() const { return username; }
};

#endif // __MINESERVER_PLAYER_H__