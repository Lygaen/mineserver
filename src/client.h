/**
 * @file client.h
 * @author Lygaen
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
#include <types/clientstate.h>
#include <entities/player.h>
#include <types/uuid.h>

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
    IMCStream *stream;
    bool isRunning;
    ClientState state;
    std::unique_ptr<std::byte[]> verifyToken;
    Player player;

    /**
     * @brief Single packet loop
     *
     */
    void loop();

    /**
     * @brief Makes player join server
     *
     * Makes the current player join the server.
     */
    void initiatePlayerJoin();

public:
    /**
     * @brief Construct a new Client object
     *
     * Wraps around a client socket to handle it.
     * @param sock the socket to wrap around
     */
    Client(const ClientSocket& sock);
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
     * Also, if in ClientState::LOGIN or ClientState::PLAY,
     * sends a disconnect packet.
     *@param reason the reason for closing the connection
     */
    void close(const std::string &reason = "");
};

#endif // MINESERVER_CLIENT_H