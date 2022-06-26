#include "server.h"
#include <utils/logger.h>
#include <thread>

Server::Server(unsigned short port)
{
    cpnet_init();

    socket = cpnet_socket(SOCK_STREAM);
    if (cpnet_bind(socket, nullptr, &port) != 0)
    {
        logger::critical("Could not bind to port {} !", port);
        logger::critical("{}", cpnet_last_error());
        exit(1);
    }

    logger::info("Server started on port {} !", port);
}

Server::~Server()
{
}

void Server::start()
{
    // TODO Maybe move to a higher number ?
    cpnet_listen(socket, 10);

    running = true;
    std::shared_ptr<bool> hasAccepted = std::make_shared<bool>();
    *hasAccepted = false;
    std::thread(clientLoop, socket, hasAccepted).detach();
    while (running)
    {
        if (*hasAccepted)
        {
            std::thread(clientLoop, socket, hasAccepted).detach();
            *hasAccepted = false;
        }
    }
}

void Server::clientLoop(socket_t socket, std::shared_ptr<bool> hasAccepted)
{
    char address[46];
    socket_t client = cpnet_accept(socket, address, nullptr);
    *hasAccepted = true;

    logger::debug("Client accepted from {}", address);

    char buffer[1024];
    ssize_t r = cpnet_read(client, buffer, 1024);
    logger::debug("Data : {}", buffer);
}