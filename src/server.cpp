#include "server.h"
#include <utils/logger.h>
#include <client.h>
#include <future>
#include <plugins/event.h>
#include <plugins/events/serverevents.hpp>

Server *Server::INSTANCE;
Server::Server() : sock(), pluginsManager(), eventsManager() // We initialize later once the network is initialized
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

    isRunning = true;

    ServerStartEvent startEvent;
    eventsManager.fire(startEvent);

    logger::info("Server started on %s:%d !", addr.c_str(), port);
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
