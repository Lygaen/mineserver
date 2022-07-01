#include <iostream>
#include <cpnet-network.h>
#include <rapidjson/rapidjson.h>
#include <utils/config.h>
#include <utils/cryptor.h>
#include <data/server.h>

int main(int argc, char **argv)
{
    Config config(argc, argv);

    // Generate keypair
    cryptor::rsaInit();

    Server server(config.port.get());
    server.start();
}
