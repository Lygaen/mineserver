#ifndef __MINESERVER_CLIENT_H__
#define __MINESERVER_CLIENT_H__

#include <cpnet-network.h>
#include <data/stream.h>

enum State
{
    HANDSHAKE,
    STATUS,
    LOGIN,
    PLAY
};

class Client
{
private:
    socket_t socket;
    Stream *streamHolder;
    StreamWrapper stream;
    State state;

    bool running;

public:
    Client(socket_t socket);
    ~Client();

    void start();
    void loop();

    void stop() { running = false; }
};

#endif // __MINESERVER_CLIENT_H__