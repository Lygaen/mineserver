#include "logger.h"
#include <cstdio>
#include <thread>

std::string logger::getCurrentTime()
{
    time_t timer;
    char buffer[26];
    struct tm *tmInfo;

    timer = time(nullptr);
    tmInfo = localtime(&timer);

    strftime(buffer, 26, "%H:%M:%S", tmInfo);

    return {buffer};
}