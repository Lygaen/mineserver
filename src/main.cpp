#include <iostream>
#include <cpnet-network.h>
#include <rapidjson/rapidjson.h>
#include <utils/config.h>
#include <data/server.h>

int main(int argc, char **argv)
{
    Config config(argc, argv);

    Server server(config.port.get());
    server.start();
}
