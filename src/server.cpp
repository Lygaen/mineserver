#include "server.h"
#include <utils/logger.h>
#include <client.h>
#include <future>

Server *Server::INSTANCE;
Server::Server() : sock() // We initialize later once the network is initialized
{
    if (INSTANCE)
        return;
    INSTANCE = this;

    if (!ServerSocket::init())
    {
        logger::fatal("Could not init networking !");
        exit(EXIT_FAILURE);
    }

    sock = ServerSocket(SOCK_STREAM);
}

Server::~Server()
{
    INSTANCE = nullptr;
    if (!ServerSocket::cleanup())
    {
        logger::error("Could not clean up properly !");
    }
}

void Server::start()
{
    std::string addr = Config::inst()->ADDRESS.getValue();
    int port = Config::inst()->PORT.getValue();

    if (!sock.bind(addr.c_str(), port))
    {
        logger::fatal("Could not start server on %s:%d", addr.c_str(), port);
        exit(EXIT_FAILURE);
    }
    sock.start(Config::inst()->BACKLOG.getValue());
    logger::info("Server started on %s:%d !", addr.c_str(), port);

    isRunning = true;
    while (isRunning)
    {
        ClientSocket cs = sock.accept();

        std::thread([&cs]()
                    { Client client(cs);
                    client.start(); })
            .detach();
    }
}

void Server::stop()
{
    logger::info("Stopping server...");
    isRunning = false;
    sock.close();
}
