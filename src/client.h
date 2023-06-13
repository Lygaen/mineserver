/**
 * @file client.h
 * @author Mathieu Cayeux
 * @brief The file containing the logic for client handling
 * @version 0.1
 * @date 2023-04-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_CLIENT_H
#define MINESERVER_CLIENT_H

#include <net/stream.h>
#include <net/types/clientstate.h>

/**
 * @brief Client class
 *
 * Class that wraps around a socket and does
 * all of the client handling.
 */
class Client
{
private:
    ClientSocket sock;
    IStream *stream;
    bool isRunning;
    ClientState state;

    /**
     * @brief Single packet loop
     *
     */
    void loop();

public:
    /**
     * @brief Construct a new Client object
     *
     * Wraps around a client socket to handle it.
     * @param sock the socket to wrap around
     */
    Client(ClientSocket sock);
    /**
     * @brief Destroy the Client object
     *
     */
    ~Client();

    /**
     * @brief Starts the client, blocking
     *
     */
    void start();
    /**
     * @brief Stops the client
     *
     * Tells the client to exit on the next packet loop.
     */
    void close();
};

#endif // MINESERVER_CLIENT_H