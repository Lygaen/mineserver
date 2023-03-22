/**
 * @file network.h
 * @author Mathieu Cayeux
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

class ClientSocket
{
private:
    socket_t sock;
    std::string address;

public:
    ClientSocket(socket_t client, char *addr);
    ~ClientSocket();

    ssize_t read(std::byte *buffer, size_t len);
    ssize_t write(std::byte *buffer, size_t len);
};

class ServerSocket
{
private:
    socket_t sock;
    bool isClient;

public:
    ServerSocket(int type);
    ~ServerSocket();

    bool bind(const char *address, int port);
    void start(unsigned int backlog);
    ClientSocket accept();
    void close();

    static bool init();
    static bool cleanup();
};

#endif