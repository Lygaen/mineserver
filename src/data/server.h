#ifndef __MINESERVER_SERVER_H__
#define __MINESERVER_SERVER_H__

#include <cpnet-network.h>
#include <memory>

class Server
{
private:
    socket_t socket;
    bool running;

    static void clientLoop(socket_t socket, std::shared_ptr<bool> hasAccepted);

public:
    Server(unsigned short port);
    ~Server();

    void start();
    void stop() { running = false; }
};

#endif // __MINESERVER_SERVER_H__