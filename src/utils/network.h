/**
 * @file network.h
 * @author Lygaen
 * @brief The network (TCP) cross-platform implementation
 * @version 1.0
 * @date 2023-03-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_NETWORK_H
#define MINESERVER_NETWORK_H

#include <string>
#if defined(__linux__)
#include <netdb.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#elif defined(_WIN32)
#include <WS2tcpip.h>
#include <Windef.h>
#include <stdio.h>
#include <io.h>
#endif

#if defined(__linux__)
typedef int socket_t;
#elif defined(_WIN32)
typedef SOCKET socket_t;
typedef SSIZE_T ssize_t;
#endif

/**
 * @brief Network POSIX Client
 *
 * Wrapper for cross-platform POSIX networking.
 */
class ClientSocket
{
private:
    bool connected;
    socket_t sock;
    std::string address;

public:
    /**
     * @brief Construct a new Client Socket object
     *
     * Constructs a client socket, only used by
     * the ::ServerSocket internally. You should
     * use other constructers instead.
     * @param client the client to attach on
     * @param addr the address on which the socket is connected
     */
    ClientSocket(socket_t client, char *addr);
    /**
     * @brief Construct a new Client Socket object
     *
     * Creates a new Socket client, to be able
     * to connect to remote addresses manually.
     * @param type the socket type, please use SOCK_STREAM
     */
    ClientSocket(int type);
    /**
     * @brief Destroy the Client Socket object
     *
     */
    ~ClientSocket() = default;

    /**
     * @brief Connects to an @p address on a certain @p port
     *
     * Keep in mind that if the ::ClientSocket was
     * constructed by the ::ServerSocket, this function
     * will fail !
     * @param address the address to connect on
     * @param port the port to connect on
     * @return true the connection was succesfull
     * @return false something went wrong
     */
    bool connect(const char *address, int port);

    /**
     * @brief Whether the connection is valid
     *
     * @return true the connection is valid
     * @return false the connection is not valid
     */
    bool isValid() const
    {
        return sock >= 0;
    }
    /**
     * @brief Reads from the socket
     *
     * Everything is said.
     * @param buffer the buffer to write into
     * @param len the maximum length to write
     * @return ssize_t the data length actually read
     */
    ssize_t read(std::byte *buffer, size_t len);
    /**
     * @brief Writes to the socket
     *
     * Everything is said.
     * @param buffer the buffer to read from
     * @param len the maximum length to read
     * @return ssize_t the data length actually written
     */
    ssize_t write(std::byte *buffer, size_t len);
    /**
     * @brief Closes the connection
     *
     * Must be run when you finish your work or
     * else it will come at a cost of leaking
     * memory !
     */
    void close();
};

/**
 * @brief Network POSIX Server
 *
 * Wrapper for cross-platform POSIX networking.
 */
class ServerSocket
{
private:
    socket_t sock;
    bool isClient;

public:
    /**
     * @brief Construct a new empty Server Socket object
     *
     * Only used for late initialization, any call on
     * an object initialized that way will fail.
     */
    ServerSocket();
    /**
     * @brief Construct a new Server Socket object
     *
     * Creates a server using the POSIX api.
     * @param type the socket type, please use SOCK_STREAM
     */
    ServerSocket(int type);
    /**
     * @brief Destroy the Server Socket object
     *
     */
    ~ServerSocket() = default;

    /**
     * @brief Binds the server to port / address
     *
     * Binds the server to a specific address and
     * port to listen on.
     * @param address the address to bind on
     * @param port the port to bind on
     * @return true binding was succesfull
     * @return false something went wrong
     */
    bool bind(const char *address, int port);
    /**
     * @brief Starts the server
     *
     * Starts listening for incoming connections,
     * with a max "acceptance" of incoming connections
     * of @p backlog
     * @param backlog the max incoming connections
     */
    void start(unsigned int backlog);
    /**
     * @brief Accepts a new client
     *
     * Accepts synchronously a new client,
     * and gives back a wrapper for the client
     * socket when the client is connected.
     * @return ClientSocket
     */
    ClientSocket accept();
    /**
     * @brief Closes the connection
     *
     * Manually closes the connection,
     * should be always ran when you finished
     * your stuff.
     */
    void close();

    /**
     * @brief Inits the POSIX API
     *
     * Because windows is not capable
     * of making correct APIs, we need
     * to initialize it, in the case we
     * are running this code on a windows
     * machine... Thanks windows !
     * It will never fail on a Linux Machine.
     * @return true API was loaded correctly
     * @return false something went wrong
     */
    static bool init();
    /**
     * @brief Cleansup the POSIX API
     *
     * Akin to ::ServerSocket::init(),
     * we need also to cleanup windows
     * mess ! Thanks again windows !
     * It will never fail on a Linux Machine.
     * @return true API was cleaned up correctly
     * @return false something went wrong
     */
    static bool cleanup();
};

#endif