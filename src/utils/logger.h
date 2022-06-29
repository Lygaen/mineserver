#ifndef __MINESERVER_LOGGER_H__
#define __MINESERVER_LOGGER_H__

#include <string>
#include <fmt/core.h>
#include <fmt/color.h>
#include <iostream>
#include <chrono>
#include <mutex>

namespace logger
{
    std::string getCurrentTime();
    static std::mutex lock;

    template <typename... Args>
    std::basic_string<char> colored(const fmt::text_style &ts, const Args &...args)
    {
#ifdef MINESERVER_ANSI_SUPPORT
        return fmt::format(ts, args...);
#else
        return fmt::format(fmt::text_style(), args...);
#endif
    }

    template <typename... T>
    void info(fmt::format_string<T...> s, T &&...args)
    {
        lock.lock();
        std::cout << colored(fmt::emphasis::bold | fmt::fg(fmt::color::blue), "[INFO]")
                  << "  " << colored(fmt::emphasis::italic | fmt::fg(fmt::color::gray), getCurrentTime())
                  << " - " << fmt::format(s, std::forward<T>(args)...)
                  << "\n";
        lock.unlock();
    }

    template <typename... T>
    void warn(fmt::format_string<T...> s, T &&...args)
    {
        lock.lock();
        std::cout << colored(fmt::emphasis::bold | fmt::fg(fmt::color::yellow), "[WARN]")
                  << "  " << colored(fmt::emphasis::italic | fmt::fg(fmt::color::gray), getCurrentTime())
                  << " - " << fmt::format(s, std::forward<T>(args)...)
                  << "\n";
        lock.unlock();
    }

    enum PacketDirection
    {
        TO_CLIENT,
        TO_SERVER
    };

    template <typename... T>
    void packet(PacketDirection direction, int len, int id, const std::string &name)
    {
#ifndef NDEBUG
        lock.lock();
        const char *dir = direction == TO_SERVER ? "C->S" : "C<-S";
        std::cout << colored(fmt::emphasis::bold | fmt::fg(fmt::color::blue_violet), "[PACK]")
                  << "  " << colored(fmt::emphasis::italic | fmt::fg(fmt::color::gray), getCurrentTime())
                  << " - " << fmt::format("{} {} (0x{:02X}) LENGTH {} bytes ", dir, name, id, len)
                  << "\n";
        lock.unlock();
#endif
    }

    template <typename... T>
    void debug(fmt::format_string<T...> s, T &&...args)
    {
#ifndef NDEBUG
        lock.lock();
        std::cout << colored(fmt::emphasis::bold | fmt::fg(fmt::color::purple), "[DEBUG]")
                  << " " << colored(fmt::emphasis::italic | fmt::fg(fmt::color::gray), getCurrentTime())
                  << " - " << fmt::format(s, std::forward<T>(args)...)
                  << "\n";
        lock.unlock();
#endif
    }

    template <typename... T>
    void critical(fmt::format_string<T...> s, T &&...args)
    {
        lock.lock();
        std::cout << colored(fmt::emphasis::bold | fmt::fg(fmt::color::red), "[CRIT]")
                  << "  " << colored(fmt::emphasis::italic | fmt::fg(fmt::color::gray), getCurrentTime())
                  << " - " << fmt::format(s, std::forward<T>(args)...)
                  << "\n";
        lock.unlock();
    }

    template <typename... T>
    void error(fmt::format_string<T...> s, T &&...args)
    {
        lock.lock();
        std::cout << colored(fmt::emphasis::bold | fmt::fg(fmt::color::deep_pink), "[ERROR]")
                  << " " << colored(fmt::emphasis::italic | fmt::fg(fmt::color::gray), getCurrentTime())
                  << " - " << fmt::format(s, std::forward<T>(args)...)
                  << "\n";
        lock.unlock();
    }
}

#endif // __MINESERVER_LOGGER_H__