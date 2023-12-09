#include <iostream>
#include <utils/config.h>
#include <server.h>
#include <csignal>

int main()
{
    Config config;

    Server server;

    std::signal(SIGINT, [](int signal)
                { (void) signal; Server::inst()->stop(); });

    server.start();
    return 0;
}
