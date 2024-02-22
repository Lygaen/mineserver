/**
 * @file main.cpp
 * @author Lygaen
 * @brief The file for the program entry point
 * @version 0.1
 * @date 2023-12-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include <utils/config.h>
#include <server.h>
#include <csignal>

/**
 * @brief Main entry point for executable
 *
 * @return int the return value
 */
int main()
{
    Config config;

    Server server;

    std::signal(SIGINT, [](int signal)
                { (void) signal; Server::inst()->stop(); });

    server.start();
    return 0;
}
