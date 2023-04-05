#include "logger.h"

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
    else LOGLEVEL = LogLevel::ALL;
#undef QCK
}

void logger::debug(const char *format, ...)
{
    if (LogLevel::DEBUG < LOGLEVEL)
        return;

    va_list args;
    va_start(args, format);

    std::string formatted = DEBUG_COLOR + std::string("[DEBUG] ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";

    std::vprintf(formatted.c_str(), args);
    va_end(args);
}

void logger::info(const char *format, ...)
{
    if (LogLevel::INFO < LOGLEVEL)
        return;

    va_list args;
    va_start(args, format);

    std::string formatted = INFO_COLOR + std::string("[INFO]  ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";
    std::vprintf(formatted.c_str(), args);
    va_end(args);
}

void logger::warn(const char *format, ...)
{
    if (LogLevel::WARN < LOGLEVEL)
        return;

    va_list args;
    va_start(args, format);

    std::string formatted = WARN_COLOR + std::string("[WARN]  ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";
    std::vprintf(formatted.c_str(), args);
    va_end(args);
}

void logger::error(const char *format, ...)
{
    if (LogLevel::ERROR < LOGLEVEL)
        return;

    va_list args;
    va_start(args, format);

    std::string formatted = ERROR_COLOR + std::string("[ERROR] ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";
    std::vprintf(formatted.c_str(), args);
    va_end(args);
}

void logger::fatal(const char *format, ...)
{
    if (LogLevel::FATAL < LOGLEVEL)
        return;

    va_list args;
    va_start(args, format);

    std::string formatted = FATAL_COLOR + std::string("[FATAL] ") + TIME_COLOR + getTime() + RESET_COLOR + " - " + format + "\n";
    std::vprintf(formatted.c_str(), args);
    va_end(args);
}
