#ifndef __MINESERVER_SERVER_H__
#define __MINESERVER_SERVER_H__

#include <cpnet-network.h>
#include <memory>

class Server
{
private:
    socket_t socket;
    bool running;

public:
    Server(unsigned short port);
    ~Server();

    void start();
    void stop() { running = false; }
};

#endif // __MINESERVER_SERVER_H__