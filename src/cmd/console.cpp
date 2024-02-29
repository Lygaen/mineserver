#include "console.h"
#include <plugins/event.h>
#include <cmd/commands.h>
#include <stdexcept>
#include <iostream>
#include <string>
#include <cstdio>
#if defined(__linux__)
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#elif defined(_WIN32)
#include <conio.h>
#include <windows.h>
#endif

ConsoleManager *ConsoleManager::instance;
ConsoleManager::ConsoleManager() : currentInput(), isRunning(false), threadHandle()
{
    if (instance)
        throw std::runtime_error("Console handler should not be constructed twice");
    instance = this;

    subscribeId = EventsManager::inst()->subscribe<logger::PostPrintEvent>(ConsoleManager::onPostPrint);
}

ConsoleManager::~ConsoleManager()
{
    if (instance == this)
        instance = nullptr;
    EventsManager::inst()->unsubscribe<logger::PostPrintEvent>(subscribeId);
}

extern "C" char getOneChar()
{
    char c;
#if defined(__linux__)
    struct termios oldattr, newattr;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
#elif defined(_WIN32)
    c = getch();
#endif
    return c;
}

inline std::string repeat(const std::string& s, int n)
{
    std::string r;
    for (int i = 0; i < n; i++)
        r += s;
    return r;
}

std::string prefix = ">";
void ConsoleManager::loop()
{
    currentInput = repeat(" ", currentInput.size());
    logger::PostPrintEvent post;
    onPostPrint(post);
    currentInput.clear();
    onPostPrint(post);

    char c;
    while (true)
    {
        c = getOneChar();
        if (c == '\n' || !isascii(c) || c < 31)
            break;
        if (c == 127)
        {
            currentInput = currentInput.substr(0, currentInput.size() - 1);
        }
        else
            currentInput += c;

        // Reprint new output
        onPostPrint(post);
    }

    std::printf("\r%s%s\n", prefix.c_str(), currentInput.c_str());
    auto res = CommandsManager::inst().callCommand(ISender::SenderType::CONSOLE, this, currentInput);
    switch (res)
    {
    case CommandsManager::COMMAND_NOT_FOUND:
        logger::error("Command not found");
        break;

    case CommandsManager::FORMAT:
        logger::error("Invalid command format");
        break;

    default:
        break;
    }
}

void ConsoleManager::start()
{
    std::thread t = std::thread([this]()
                                { 
                    this->isRunning = true;
                    while (isRunning)
                    {
                        this->loop();
                    } });
    threadHandle = t.native_handle();
    t.detach();
}

void ConsoleManager::stop()
{
    isRunning = false;

#if defined(__linux__)
    pthread_cancel(threadHandle);
#elif defined(_WIN32)
    TerminateThread((void*)threadHandle, 1);
#endif
}

void ConsoleManager::onPostPrint(logger::PostPrintEvent event)
{
    (void)event;

    std::string in = ConsoleManager::inst().currentInput;
    std::string spaces = repeat(" ", prefix.size() + in.size() + 1);

    std::printf("\r%s", spaces.c_str());
    std::printf("\r%s%s", prefix.c_str(), in.c_str());
    std::cout.flush();
}

void ConsoleManager::sendMessage(const ChatMessage &message)
{
    std::stringstream ss(message.text);
    std::string line;

    while (std::getline(ss, line, '\n'))
        logger::info("%s", line.c_str());
}
