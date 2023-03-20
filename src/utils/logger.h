/**
 * @file logger.h
 * @author Mathieu Cayeux
 * @brief The main file for the logger
 * @version 1.0
 * @date 2023-03-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_LOGGER_H
#define MINESERVER_LOGGER_H

#include <utils/config.h>
#include <cstdarg>
#include <ctime>

#ifdef MINESERVER_ANSI_COLORS
#define RESET_COLOR "\033[0m"
#define TIME_COLOR "\033[0;90m"
#define FATAL_COLOR "\033[1;31m"
#define ERROR_COLOR "\033[0;31m"
#define WARN_COLOR "\033[0;33m"
#define INFO_COLOR "\033[0;35m"
#define DEBUG_COLOR "\033[0;36m"
#else
#define RESET_COLOR ""
#define TIME_COLOR ""
#define FATAL_COLOR ""
#define ERROR_COLOR ""
#define WARN_COLOR ""
#define INFO_COLOR ""
#define DEBUG_COLOR ""
#endif

/**
 * @brief The LogLevel used internally by the logger.
 *
 * The standard log levels in an enum, only used internally
 * by the logger.
 */
enum LogLevel
{
    /**
     * @brief All level, just for enabling
     *
     * Enables all possible log levels
     */
    ALL = 0,
    /**
     * @brief Debug level
     *
     * see logger::debug() for more info
     */
    DEBUG = 1,
    /**
     * @brief Info level
     *
     * see logger::info() for more info
     */
    INFO = 2,
    /**
     * @brief warn level
     *
     * see logger::warn() for more info
     */
    WARN = 3,
    /**
     * @brief Error level
     *
     * see logger::error() for more info
     */
    ERROR = 4,
    /**
     * @brief Fatal level
     *
     * see logger::fatal() for more info
     */
    FATAL = 5,
    /**
     * @brief Off level, just for disabling
     *
     * Disbles all possible log levels
     */
    OFF = 6,
};

/**
 * @brief The logging static class
 *
 * The logging class, should NOT be manually
 * created or anything. It is fully static.
 */
class logger
{
public:
    /**
     * @brief The LogLevel for the logger to use.
     *
     * The minimum log level of the logger.
     */
    static LogLevel LOGLEVEL;

    /**
     * @brief Get the Time as a string.
     *
     * Gets the local time and returns a string representation of the time accompanied
     * by the date and the month. Used for the console logging.
     * @return std::string the ascii representation of the local time as "day/month 24:60:60"
     */
    static std::string getTime();

    /**
     * @brief Loads the loglevel from the config.
     *
     * Loads the loglevel from the config and parse it to be usable,
     * should only be used internally as a hook after the config
     * was loaded, which is in Config::load().
     */
    static void loadConfig();

    /**
     * @brief Logs something at the ::DEBUG level
     *
     * In the end is just a wrapper around printf, so it
     * will works like it.
     * Should be used to log information only used by developers.
     * @param format the format string to parse arguements in
     * @param ... the arguments for the format
     */
    static void debug(const char *format, ...);

    /**
     * @brief Logs something at the ::INFO level
     *
     * In the end is just a wrapper around printf, so it
     * will works like it.
     * Should be used to print out general information
     * such as the current status of the program and
     * so on and so on.
     * @param format the format string to parse arguements in
     * @param ... the arguments for the format
     */
    static void info(const char *format, ...);

    /**
     * @brief Logs something at the ::WARN level
     *
     * In the end is just a wrapper around printf, so it
     * will works like it.
     * Should be used to log information that the end user
     * should be warned about, but has no impact.
     * @param format the format string to parse arguements in
     * @param ... the arguments for the format
     */
    static void warn(const char *format, ...);

    /**
     * @brief Logs something at the ::ERROR level
     *
     * In the end is just a wrapper around printf, so it
     * will works like it.
     * Should only be used to logs errors to the end user,
     * when the program has done / detected something wrong,
     * that impacts the end user.
     * @param format the format string to parse arguements in
     * @param ... the arguments for the format
     */
    static void error(const char *format, ...);

    /**
     * @brief Logs something at the ::FATAL level
     *
     * In the end is just a wrapper around printf, so it
     * will works like it.
     * Should only be used when a fatal error has happened,
     * that crashes the program or has a severe impact for
     * the well-being of the program.
     * @param format the format string to parse arguements in
     * @param ... the arguments for the format
     */
    static void fatal(const char *format, ...);
};

#endif // MINESERVER_LOGGER_H
