/**
 * @file console.h
 * @author Lygaen
 * @brief The file handling console logic
 * @version 0.1
 * @date 2024-02-09
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MINESERVER_CONSOLE_H
#define MINESERVER_CONSOLE_H

#include <thread>
#include <atomic>
#include <utils/logger.h>

/**
 * @brief Console manager
 *
 */
class ConsoleManager
{
private:
    static ConsoleManager *instance;

    std::string currentInput;
    EventHandler<logger::PostPrintEvent>::subId subscribeId;
    std::thread::native_handle_type threadHandle;
    std::atomic<bool> isRunning;

    void loop();

public:
    /**
     * @brief Construct a new Console Manager object
     *
     */
    ConsoleManager();
    /**
     * @brief Destroy the Console Manager object
     *
     */
    ~ConsoleManager();

    /**
     * @brief Starts listening for commands
     *
     */
    void start();
    /**
     * @brief Stops listening for commands
     *
     */
    void stop();
    /**
     * @brief Post print hook
     *
     * So that we can print the currently
     * written input at the end of the IO.
     * @param event the post print event
     */
    static void onPostPrint(logger::PostPrintEvent event);

    /**
     * @brief Gets Console Manager instance
     *
     * @return ConsoleManager& the instance
     */
    static ConsoleManager &inst()
    {
        return *instance;
    }
};

#endif // MINESERVER_CONSOLE_H