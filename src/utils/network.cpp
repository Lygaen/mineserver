#include "network.h"
#include <cstring>
#include <stdexcept>
#ifdef _WIN32
#include <basetsd.h>
#include <WinSock2.h>
#endif // _WIN32
#if defined(__linux__)
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#endif // __linux__

ServerSocket::ServerSocket(int type)
{
    sock = socket(AF_INET, type, 0);
}

ServerSocket::~ServerSocket()
{
#if defined(_WIN32)
    closesocket(sock);
#elif defined(__linux__)
    ::close(sock);
#endif
}

bool ServerSocket::bind(const char *address, int port)
{
    if (!sock)
    {
        return -1;
    }
    struct sockaddr_in serv_addr;
    std::memset((void *)&serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    if (address)
#if defined(__linux__)
        inet_aton(address, &serv_addr.sin_addr);
#elif defined(_WIN32)
        inet_pton(AF_INET, address, &serv_addr.sin_addr);
#endif
    else
    {
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    serv_addr.sin_port = htons(port);
    int status_code = ::bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    return status_code >= 0;
}

void ServerSocket::start(unsigned int backlog)
{
    listen(sock, backlog);
}

ClientSocket ServerSocket::accept()
{
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    socket_t rvalue = ::accept(sock, (struct sockaddr *)&cli_addr, &clilen);
    char addr[46];
#if defined(__linux__)
    char *tmp = inet_ntoa(cli_addr.sin_addr);
    strcpy(addr, tmp);
#elif defined(_WIN32)
    char buffer[46];
    inet_ntop(AF_INET, &cli_addr.sin_addr, buffer, 46);
    strcpy(addr, buffer);
#endif

    return ClientSocket(rvalue, addr);
}

bool ServerSocket::init()
{
#ifdef _WIN32
    WORD requested_version;
    WSADATA wsa_data;
    int err;
    requested_version = MAKEWORD(2, 2);
    err = WSAStartup(requested_version, &wsa_data);
    if (err != 0)
    {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        return false;
    }
    /* Confirm that the WinSock DLL supports 2.2.        */
    /* Note that if the DLL supports versions greater    */
    /* than 2.2 in addition to 2.2, it will still return */
    /* 2.2 in wVersion since that is the version we      */
    /* requested.                                        */

    if (LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 2)
    {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
        return false;
    }
    else
    {
        return true;
    }
#endif
    return true;
}

bool ServerSocket::cleanup()
{
#ifdef _WIN32
    return WSACleanup() == 0;
#else
    return true;
#endif /* _WIN32 */
}

ClientSocket::ClientSocket(socket_t client, char *addr) : address(addr)
{
    sock = client;
}

ClientSocket::~ClientSocket()
{
#if defined(_WIN32)
    closesocket(sock);
#elif defined(__linux__)
    ::close(sock);
#endif
}

ssize_t ClientSocket::read(std::byte *buffer, size_t len)
{
#if defined(__linux__)
    ssize_t retval = recv(sock, buffer, len, MSG_NOSIGNAL);
#elif defined(_WIN32)
    ssize_t retval = (ssize_t)(recv(sock, buffer, (int)len, 0));
#endif
    if (retval <= 0)
        throw new std::runtime_error("Invalid socket read !");
    return retval;
}

ssize_t ClientSocket::write(std::byte *buffer, size_t len)
{
#if defined(__linux__)
    return send(sock, buffer, len, MSG_NOSIGNAL);
#elif defined(_WIN32)
    return send(sock, buffer, (unsigned int)len, 0);
#endif
}
