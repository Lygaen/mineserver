#include "console.h"
#include <plugins/event.h>
#include <cmd/commands.h>
#include <stdexcept>
#include <iostream>
#include <string>
#include <stdio.h>
#if defined(__linux__)
#include <termios.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <conio.h>
#endif

ConsoleManager *ConsoleManager::instance;
ConsoleManager::ConsoleManager() : currentInput("")
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

inline std::string repeat(std::string s, int n)
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
        if (c == '\n')
            break;
        if (c == 127 || c == 8)
            currentInput = currentInput.substr(0, currentInput.size() - 1);
        else
            currentInput += c;

        // Reprint new output
        onPostPrint(post);
    }

    std::printf("\r%s%s\n", prefix.c_str(), currentInput.c_str());
    auto res = CommandsManager::inst().callCommand(ISender::SenderType::CONSOLE, nullptr, currentInput);
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
    std::thread([this]()
                { while(true) {
                                    this->loop();
                                    } })
        .detach();
}

void ConsoleManager::onPostPrint(logger::PostPrintEvent event)
{
    std::string in = ConsoleManager::inst().currentInput;
    std::string spaces = repeat(" ", prefix.size() + in.size() + 1);

    std::printf("\r%s", spaces.c_str());
    std::printf("\r%s%s", prefix.c_str(), in.c_str());
    std::cout.flush();
}
