#include <iostream>
#include <utils/config.h>
#include <server.h>

int main()
{
    Config config;

    Server server;
    server.start();
    return 0;
}
