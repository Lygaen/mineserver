/**
 * @file server.cpp
 * @author Lygaen
 * @brief the file containing the logic for the server
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "server.h"
#include <future>
#include <utils/logger.h>
#include <plugins/event.h>
#include <plugins/events/serverevents.hpp>
#include <chrono>

Server *Server::INSTANCE;
Server::Server() : sock(),
                   connectedClients(),
                   pluginsManager(),
                   eventsManager(),
                   commandsManager(),
                   consoleManager(),
                   running(false)
{
    if (INSTANCE)
        return;
    INSTANCE = this;

    if (!ServerSocket::init())
    {
        logger::fatal("Could not init networking !");
        exit(EXIT_FAILURE);
    }

    if (!crypto::init())
    {
        logger::fatal("Could not init cryptography !");
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

void Server::checks()
{
    const PNGFile &icon = Config::inst()->ICON_FILE.getValue();
    if (icon.getHeight() != icon.getWidth() || icon.getHeight() != 64)
    {
        // Notchian clients only render 64x64 images
        logger::warn("Invalid image ! Check it's resolution (must be 64x64) or just if it's there !");
        Config::inst()->ICON_FILE.setValue(PNGFile()); // Frees memory and deletes config entry
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

    checks();

    pluginsManager.load();
    consoleManager.start();

    running = true;

    ServerStartEvent startEvent;
    eventsManager.fire(startEvent);

    logger::info("Server started on %s:%d !", addr.c_str(), port);
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // So that we don't overload the CPU
        ClientSocket cs = sock.accept();

        if (!cs.isValid())
            continue;

        // Join thread afterwards
        std::thread([&cs, this]()
                    {
            Client client(cs);
            this->connectedClients.push_back(&client);
            client.start();
            this->connectedClients.remove(&client); })
            .detach();
    }
}

void Server::stop()
{
    logger::info("Stopping server...");
    running = false;

    for (auto client : connectedClients)
    {
        client->close("Server closing");
    }

    sock.close();

    consoleManager.stop();
    logger::debug("Stopped server !");
}
