/**
 * @file server.h
 * @author Mathieu Cayeux
 * @brief The file containing most of the server logic
 * @version 0.1
 * @date 2023-04-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_SERVER_H
#define MINESERVER_SERVER_H

#include <utils/network.h>

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
    ServerSocket sock;
    bool isRunning{};

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