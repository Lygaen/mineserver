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

enum LogLevel
{
    ALL = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5,
    OFF = 6,
};

namespace logger
{
    static LogLevel LOGLEVEL = LogLevel::ALL;

    inline std::string getTime()
    {
        std::time_t now = std::time(nullptr);
        std::tm *t = std::localtime(&now);
        char buf[80];
        std::strftime(buf, 80, "%d/%m %X", t);
        return buf;
    }

    inline void loadConfig()
    {
        std::string lvl = Config::inst()->LOGLEVEL.getValue();

#define QCK(X)      \
    if (lvl == "X") \
    LOGLEVEL = LogLevel::X
        QCK(ALL);
        QCK(DEBUG);
        QCK(INFO);
        QCK(WARN);
        QCK(ERROR);
        QCK(FATAL);
        QCK(OFF);
#undef QCK
    }

    inline void debug(const char *format, ...)
    {
        if (LogLevel::DEBUG < LOGLEVEL)
            return;

        va_list args;
        va_start(args, format);

        std::string formatted = DEBUG_COLOR + std::string("[DEBUG] ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";

        std::vprintf(formatted.c_str(), args);
        va_end(args);
    }

    inline void info(const char *format, ...)
    {
        if (LogLevel::INFO < LOGLEVEL)
            return;

        va_list args;
        va_start(args, format);

        std::string formatted = INFO_COLOR + std::string("[INFO]  ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";
        std::vprintf(formatted.c_str(), args);
        va_end(args);
    }

    inline void warn(const char *format, ...)
    {
        if (LogLevel::WARN < LOGLEVEL)
            return;

        va_list args;
        va_start(args, format);

        std::string formatted = WARN_COLOR + std::string("[WARN]  ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";
        std::vprintf(formatted.c_str(), args);
        va_end(args);
    }

    inline void error(const char *format, ...)
    {
        if (LogLevel::ERROR < LOGLEVEL)
            return;

        va_list args;
        va_start(args, format);

        std::string formatted = ERROR_COLOR + std::string("[ERROR] ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";
        std::vprintf(formatted.c_str(), args);
        va_end(args);
    }

    inline void fatal(const char *format, ...)
    {
        if (LogLevel::FATAL < LOGLEVEL)
            return;

        va_list args;
        va_start(args, format);

        std::string formatted = FATAL_COLOR + std::string("[FATAL] ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";
        std::vprintf(formatted.c_str(), args);
        va_end(args);
    }
}

#endif // MINESERVER_LOGGER_H
