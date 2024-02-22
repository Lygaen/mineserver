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
#include <unordered_map>

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
    CHECK_ENUM(PLUGIN);
    CHECK_ENUM(WARN);
    CHECK_ENUM(ERROR);
    CHECK_ENUM(FATAL);
    CHECK_ENUM(OFF);
    else LOGLEVEL = LogLevel::ALL;
#undef QCK

#endif // DOXYGEN_IGNORE_THIS
}

/**
 * @brief Mappings for loglevel enum
 *
 * For ease of use, instead of having a
 * giant switch case, we have this map.
 * The first part of the pair in the values
 * is the string representation of the level
 * and the second one is the color associated
 * with that same level.
 */
const std::unordered_map<LogLevel, std::pair<std::string, std::string>> LEVELS{
    {LogLevel::DEBUG, {"DEBUG", DEBUG_COLOR}},
    {LogLevel::ERROR, {"ERROR", ERROR_COLOR}},
    {LogLevel::FATAL, {"FATAL", FATAL_COLOR}},
    {LogLevel::INFO, {"INFO", INFO_COLOR}},
    {LogLevel::PLUGIN, {"PLUGIN", PLUGIN_COLOR}},
    {LogLevel::WARN, {"WARN", WARN_COLOR}},
};

#ifdef MINESERVER_ANSI_COLORS

const std::unordered_map<char, std::string> NOTCHIAN_TO_ANSI{
    {'0', "\033[90m"}, // Not actual black or else won't see
    {'1', "\033[34m"},
    {'2', "\033[32m"},
    {'3', "\033[36m"},
    {'4', "\033[31m"},
    {'5', "\033[35m"},
    {'6', "\033[33m"},
    {'7', "\033[37m"},
    {'8', "\033[90m"},
    {'9', "\033[94m"},
    {'a', "\033[92m"},
    {'b', "\033[96m"},
    {'c', "\033[91m"},
    {'d', "\033[95m"},
    {'e', "\033[93m"},
    {'f', "\033[97m"},
    {'k', RESET_COLOR}, // No ANSI equivalent
    {'l', "\033[1m"},
    {'m', "\033[9m"},
    {'n', "\033[24m"},
    {'o', "\033[3m"},
    {'r', RESET_COLOR},
};

std::string replaceMinecraftEscapes(const std::string &s)
{
    std::string res = s;
    auto getNextPos = [&res](size_t pos)
    {
        size_t p1 = res.find("§", 0);
        size_t p2 = res.find('&', 0);
        return std::min(p1, p2);
    };

    size_t pos = 0;
    while ((pos = getNextPos(pos)) != std::string::npos)
    {
        if (res.at(pos) != '&')
            res.erase(pos, 2); // § symbol is 2-bytes long
        else
            res.erase(pos, 1);

        char code = res.at(pos);
        if (!NOTCHIAN_TO_ANSI.contains(code))
            continue;

        res.erase(pos, 1);
        res.insert(pos, NOTCHIAN_TO_ANSI.at(code));
    }

    return res;
}

#endif // MINESERVER_ANSI_COLORS

/**
 * @brief Logs a format and its args at a certain level
 *
 * @param level the level to log to
 * @param format the format of the log
 * @param args the arguments of the log
 */
void logAtLevel(LogLevel level, std::string format, va_list args)
{
    if (level < LOGLEVEL)
        return;

    const std::pair<std::string, std::string> &levelInfo = LEVELS.at(level);
    std::string levelString = levelInfo.first;
    std::string color = levelInfo.second;

#ifdef MINESERVER_ANSI_COLORS
    format = replaceMinecraftEscapes(format);
#endif // MINESERVER_ANSI_COLORS

    constexpr int MAX_LEVEL_NAME_SIZE = 6;
    std::string spaces(MAX_LEVEL_NAME_SIZE - levelString.size(), ' ');

    std::string formatted = color + "\r[" + levelString + "] " + spaces + TIME_COLOR + logger::getTime() + RESET_COLOR + " - " + format + RESET_COLOR + "\n";

    std::vprintf(formatted.c_str(), args);

    if (EventsManager::inst() == nullptr)
        return;

    logger::PostPrintEvent event;
    EventsManager::inst()->fire(event);
}

void logger::debug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logAtLevel(LogLevel::DEBUG, format, args);
    va_end(args);
}

void logger::info(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logAtLevel(LogLevel::INFO, format, args);
    va_end(args);
}

void logger::plugin(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logAtLevel(LogLevel::PLUGIN, format, args);
    va_end(args);
}

void logger::warn(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logAtLevel(LogLevel::WARN, format, args);
    va_end(args);
}

void logger::error(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logAtLevel(LogLevel::ERROR, format, args);
    va_end(args);
}

void logger::fatal(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logAtLevel(LogLevel::FATAL, format, args);
    va_end(args);
}
