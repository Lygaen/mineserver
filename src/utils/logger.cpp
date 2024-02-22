/**
 * @file logger.cpp
 * @author Lygaen
 * @brief The file containing the logic for the logger
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "logger.h"
#include "plugins/luaheaders.h"

/**
 * @brief The current stored loglevel
 *
 */
LogLevel LOGLEVEL;

std::string logger::getTime()
{
    std::time_t now = std::time(nullptr);
    std::tm *t = std::localtime(&now);
    char buf[80];
    std::strftime(buf, 80, "%d/%m %X", t);
    return buf;
}

void logger::loadConfig()
{
    std::string lvl = Config::inst()->LOGLEVEL.getValue();
#ifndef DOXYGEN_IGNORE_THIS

#define CHECK_ENUM(X) \
    if (lvl == #X)    \
    LOGLEVEL = LogLevel::X
    CHECK_ENUM(ALL);
    CHECK_ENUM(DEBUG);
    CHECK_ENUM(INFO);
    CHECK_ENUM(WARN);
    CHECK_ENUM(ERROR);
    CHECK_ENUM(FATAL);
    CHECK_ENUM(OFF);
    else LOGLEVEL = LogLevel::ALL;
#undef QCK

#endif // DOXYGEN_IGNORE_THIS
}

void logger::debug(const char *format, ...)
{
    if (LogLevel::DEBUG < LOGLEVEL)
        return;

    va_list args;
    va_start(args, format);

    std::string formatted = DEBUG_COLOR + std::string("\r[DEBUG]  ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";
    std::vprintf(formatted.c_str(), args);
    va_end(args);

    if (EventsManager::inst() == nullptr)
        return;

    PostPrintEvent event;
    EventsManager::inst()->fire(event);
}

void logger::info(const char *format, ...)
{
    if (LogLevel::INFO < LOGLEVEL)
        return;

    va_list args;
    va_start(args, format);

    std::string formatted = INFO_COLOR + std::string("\r[INFO]   ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";
    std::vprintf(formatted.c_str(), args);
    va_end(args);

    if (EventsManager::inst() == nullptr)
        return;

    PostPrintEvent event;
    EventsManager::inst()->fire(event);
}

void logger::plugin(const char *format, ...)
{
    if (LogLevel::INFO < LOGLEVEL)
        return;

    va_list args;
    va_start(args, format);

    std::string formatted = PLUGIN_COLOR + std::string("\r[PLUGIN] ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";
    std::vprintf(formatted.c_str(), args);
    va_end(args);

    if (EventsManager::inst() == nullptr)
        return;

    PostPrintEvent event;
    EventsManager::inst()->fire(event);
}

void logger::warn(const char *format, ...)
{
    if (LogLevel::WARN < LOGLEVEL)
        return;

    va_list args;
    va_start(args, format);

    std::string formatted = WARN_COLOR + std::string("\r[WARN]   ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";
    std::vprintf(formatted.c_str(), args);
    va_end(args);

    if (EventsManager::inst() == nullptr)
        return;

    PostPrintEvent event;
    EventsManager::inst()->fire(event);
}

void logger::error(const char *format, ...)
{
    if (LogLevel::ERROR < LOGLEVEL)
        return;

    va_list args;
    va_start(args, format);

    std::string formatted = ERROR_COLOR + std::string("\r[ERROR]  ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";
    std::vprintf(formatted.c_str(), args);
    va_end(args);

    if (EventsManager::inst() == nullptr)
        return;

    PostPrintEvent event;
    EventsManager::inst()->fire(event);
}

void logger::fatal(const char *format, ...)
{
    if (LogLevel::FATAL < LOGLEVEL)
        return;

    va_list args;
    va_start(args, format);

    std::string formatted = FATAL_COLOR + std::string("\r[FATAL]  ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";
    std::vprintf(formatted.c_str(), args);
    va_end(args);

    if (EventsManager::inst() == nullptr)
        return;

    PostPrintEvent event;
    EventsManager::inst()->fire(event);
}
