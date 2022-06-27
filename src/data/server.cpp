#include "server.h"
#include <utils/logger.h>
#include <thread>
#include <data/client.h>

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
    while (running)
    {
        char address[46];
        socket_t scl = cpnet_accept(socket, address, nullptr);

        logger::debug("Client accepted from {}", address);

        Client client(scl);
        client.start();

        logger::debug("Client disconnected from {}", address);
    }
}