/**
 * @file server.h
 * @author Lygaen
 * @brief The file containing most of the server logic
 * @version 0.1
 * @date 2023-04-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_SERVER_H
#define MINESERVER_SERVER_H

#include <plugins/plugins.h>
#include <plugins/event.h>
#include <utils/network.h>
#include <types/chatmessage.h>
#include <cmd/commands.h>
#include <cmd/console.h>
#include <client.h>
#include <atomic>
#include <list>

/**
 * @brief Server Class
 *
 * Class that handles mainly networking and
 * batches everything together.
 * The server is a singleton.
 */
class Server
{
private:
    static Server *INSTANCE;
    std::list<Client *> connectedClients;
    PluginsManager pluginsManager;
    EventsManager eventsManager;
    CommandsManager commandsManager;
    ConsoleManager consoleManager;
    ServerSocket sock;
    std::atomic<bool> running;

    /**
     * @brief Internal Checks
     *
     * Internal checks that will not
     * prevent the integrity of the
     * server.
     */
    static void checks();

public:
    /**
     * @brief Construct a new Server object
     *
     */
    Server();
    /**
     * @brief Destroy the Server object
     *
     */
    ~Server();

    /**
     * @brief Starts the server, blocking
     *
     * Starts the client loop, binding to the config's
     * port and address.
     */
    void start();
    /**
     * @brief Stops the server
     *
     * Stops the server on the next client loop (eg when a client
     * connects).
     */
    void stop();

    bool isRunning() const {
        return running;
    }

    /**
     * @brief Get the instance of the server
     *
     * Simple singleton implementation.
     * @return Server* a pointer to the server.
     */
    static Server *inst()
    {
        return INSTANCE;
    }
};

#endif // MINESERVER_SERVER_H