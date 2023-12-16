/**
 * @file logger.h
 * @author Lygaen
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
/**
 * @brief ANSI reset color
 *
 */
#define RESET_COLOR "\033[0m"
/**
 * @brief ANSI color for time in the console
 *
 * See logger::getTime() for more info
 */
#define TIME_COLOR "\033[0;90m"
/**
 * @brief ANSI color for fatal level in the console
 *
 */
#define FATAL_COLOR "\033[1;31m"
/**
 * @brief ANSI color for error level in the console
 *
 * See logger::error() for more info
 */
#define ERROR_COLOR "\033[0;31m"
/**
 * @brief ANSI color for warn level in the console
 *
 * See logger::warn() for more info
 */
#define WARN_COLOR "\033[0;33m"
/**
 * @brief ANSI color for plugins
 *
 * Same level as LogLevel::INFO
 * See logger::plugin() for more info
 */
#define PLUGIN_COLOR "\033[0;34m"
/**
 * @brief ANSI color for info level in the console
 *
 * See logger::info() for more info
 */
#define INFO_COLOR "\033[0;35m"
/**
 * @brief ANSI color for debug level in the console
 *
 * See logger::debug() for more info
 */
#define DEBUG_COLOR "\033[0;36m"
#else
#define RESET_COLOR ""
#define TIME_COLOR ""
#define FATAL_COLOR ""
#define ERROR_COLOR ""
#define WARN_COLOR ""
#define PLUGIN_COLOR ""
#define INFO_COLOR ""
#define DEBUG_COLOR ""
#endif

// Because windows already uses the name
#undef ERROR

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
     * @brief Info and Plugin level
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
 * @brief The logging namespace
 *
 * The logging namespace, contains all
 * of the logging-related functions.
 */
namespace logger
{
    /**
     * @brief Get the Time as a string.
     *
     * Gets the local time and returns a string representation of the time accompanied
     * by the date and the month. Used for the console logging.
     * @return std::string the ascii representation of the local time as "day/month 24:60:60"
     */
    std::string getTime();

    /**
     * @brief Loads the loglevel from the config.
     *
     * Loads the loglevel from the config and parse it to be usable,
     * should only be used internally as a hook after the config
     * was loaded, which is in Config::load().
     */
    void loadConfig();

    /**
     * @brief Logs something at the ::DEBUG level
     *
     * In the end is just a wrapper around printf, so it
     * will works like it.
     * Should be used to log information only used by developers.
     * @param format the format string to parse arguements in
     * @param ... the arguments for the format
     */
    void debug(const char *format, ...);

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
    void info(const char *format, ...);

    /**
     * @brief Logs something at the ::INFO level, for plugins
     *
     * In the end is just a wrapper around printf, so it
     * will works like it.
     * Should be used to print out general information
     * such as the current status of the program and
     * so on and so on.
     * @param format the format string to parse arguements in
     * @param ... the arguments for the format
     */
    void plugin(const char *format, ...);

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
    void warn(const char *format, ...);

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
    void error(const char *format, ...);

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
    void fatal(const char *format, ...);
};

#endif // MINESERVER_LOGGER_H
