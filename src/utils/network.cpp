/**
 * @file network.cpp
 * @author Lygaen
 * @brief The file containing the logic for the networking
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "network.h"
#include <cstring>
#include <stdexcept>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <rapidjson/document.h>
#ifdef _WIN32
#include <basetsd.h>
#include <WinSock2.h>
#endif // _WIN32
#if defined(__linux__)
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/ioctl.h>
#endif // __linux__

ServerSocket::ServerSocket() = default;

ServerSocket::ServerSocket(int type)
{
    sock = socket(AF_INET, type, 0);
}

bool ServerSocket::bind(const char *address, int port) const
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
#if defined(__linux__)
    fcntl(sock, F_SETFL, O_NONBLOCK);
#elif defined(_WIN32)
    u_long mode = 1; // 1 to enable non-blocking socket
    ioctlsocket(sock, FIONBIO, &mode);
#endif
    int status_code = ::bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    return status_code >= 0;
}

void ServerSocket::start(unsigned int backlog) const
{
    listen(sock, backlog);
}

ClientSocket ServerSocket::accept() const
{
    struct sockaddr_in cli_addr{};
    socklen_t cliLen = sizeof(cli_addr);
    socket_t rvalue = ::accept(sock, (struct sockaddr *)&cli_addr, &cliLen);
    char addr[46];
    char *tmp = inet_ntoa(cli_addr.sin_addr);
#if defined(__linux__)
    strncpy(addr, tmp, 46);
#elif defined(_WIN32)
    char buffer[46];
    inet_ntop(AF_INET, &cli_addr.sin_addr, buffer, 46);
    strncpy(addr, tmp, 46);
#endif

    return {rvalue, addr};
}

void ServerSocket::close() const
{
#if defined(_WIN32)
    closesocket(sock);
#elif defined(__linux__)
    ::close(sock);
#endif
}

bool ServerSocket::init()
{
#if defined(__linux__)
    return true;
#elif defined(_WIN32)
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
    /* then 2.2 in addition to 2.2, it will still return */
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
    connected = true;

#if defined(__linux__)
    const int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags & ~O_NONBLOCK);
#elif defined(_WIN32)
    u_long mode = 0; // 0 to disable non-blocking socket
    ioctlsocket(sock, FIONBIO, &mode);
#endif
}

ClientSocket::ClientSocket(int type)
{
    connected = false;

#if defined(__linux__)
    const int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags & ~O_NONBLOCK);
#elif defined(_WIN32)
    u_long mode = 0; // 0 to disable non-blocking socket
    ioctlsocket(sock, FIONBIO, &mode);
#endif
}

bool ClientSocket::connect(const char *address, int port) const
{
    if (connected)
        return false;

    struct sockaddr_in serv_addr{};
    struct hostent *server;
    server = gethostbyname(address);
    if (server == nullptr)
    {
        return -1;
    }
    memset((void *)&serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memmove((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    serv_addr.sin_port = htons(port);
    return ::connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == 0;
}

ssize_t ClientSocket::read(std::byte *buffer, size_t len) const
{
#if defined(__linux__)
    ssize_t retval = recv(sock, buffer, len, MSG_NOSIGNAL);
    if (retval <= 0)
        throw std::runtime_error("Invalid socket read !");
#elif defined(_WIN32)
    auto retval = (ssize_t)(recv(sock, reinterpret_cast<char*>(buffer), (int)len, 0));
    if (retval == SOCKET_ERROR)
        throw std::runtime_error("Invalid socket read (" + std::to_string(WSAGetLastError()) + ")");
#endif
    return retval;
}

ssize_t ClientSocket::write(const std::byte *buffer, size_t len) const
{
#if defined(__linux__)
    return send(sock, buffer, len, MSG_NOSIGNAL);
#elif defined(_WIN32)
    return send(sock, reinterpret_cast<const char*>(buffer), (unsigned int)len, 0);
#endif
}

void ClientSocket::close() const
{
#if defined(_WIN32)
    closesocket(sock);
#elif defined(__linux__)
    ::close(sock);
#endif
}

size_t ClientSocket::getAvailableBytes() const
{
#if defined(_WIN32)
    u_long available = 0;
    ioctlsocket(sock, FIONREAD, &available);
#elif defined(__linux__)
    int available = 0;
    ioctl(sock, FIONREAD, &available);
#endif
    return available;
}

bool ClientSocket::isValid() const {
    bool isValid;
#if defined(_WIN32)
    isValid = sock != INVALID_SOCKET;
#elif defined(__linux__)
    isValid = sock >= 0;
#endif
    return isValid;
}

mojangapi::HasJoinedResponse mojangapi::hasJoined(const std::string &username, const std::string &serverId, const std::string &ip)
{
    OpenSSL_add_ssl_algorithms();
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx)
        throw std::runtime_error("Could not initialize SSL context");

    ClientSocket socket(SOCK_STREAM);
    if (!socket.connect("sessionserver.mojang.com", 443))
        throw std::runtime_error("Could not connect to server");

    std::string url = std::string("/session/minecraft/hasJoined") + "?username=" + username + "&serverId=" + serverId + (ip.empty() ? "" : ("&ip=" + ip));

    SSL *ssl = SSL_new(ctx);
    if (!ssl)
        throw std::runtime_error("Could not initialize SSL");
    SSL_set_fd(ssl, socket.getHandle());

    if (SSL_connect(ssl) == -1)
        throw std::runtime_error("Could not initialize SSL");

    std::string s = "GET " + url + " HTTP/1.1\r\nHost:sessionserver.mojang.com\r\n\r\n";

    if (SSL_write(ssl, s.c_str(), s.size()) == -1)
        throw std::runtime_error("Could not write to SSL");

    char buff[8192];
    int len = SSL_read(ssl, buff, sizeof(buff) - 1);
    if (len == -1)
        throw std::runtime_error("Could not read to SSL");
    buff[len] = '\0';

    std::string response(buff);
    auto loc = response.find("\r\n\r\n");
    response = response.substr(loc + 4);

    SSL_shutdown(ssl);
    socket.close();
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    HasJoinedResponse r;
    rapidjson::Document doc;
    doc.Parse(response.c_str());

    if (doc.HasParseError() || !doc.IsObject() ||
        !doc.HasMember("id") || !doc["id"].IsString() ||
        !doc.HasMember("name") || !doc["name"].IsString())
        throw std::runtime_error("Could not parse JSON response");

    r.name = std::string(doc["name"].GetString(), doc["name"].GetStringLength());
    r.id = MinecraftUUID::fromHex(std::string(doc["id"].GetString(), doc["id"].GetStringLength()));

    return r;
}
