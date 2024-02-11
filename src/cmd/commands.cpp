#include "commands.h"
#include <utils/logger.h>
#include <cmd/commandsreg.hpp>

CommandsManager *CommandsManager::instance = nullptr;
CommandsManager::CommandsManager() : commands()
{
    if (instance)
        throw std::runtime_error("Commands handler should not be constructed twice");

    instance = this;

    registerCommands();
}

CommandsManager::~CommandsManager()
{
    if (instance == this)
        instance = nullptr;
}

static std::regex nameRegex("[a-z][a-z-A-Z]+");
void CommandsManager::addCommand(const Command &command)
{
    if (command.name.empty() || !std::regex_match(command.name, nameRegex))
    {
        logger::warn("Could not register command '%s'", command.name.c_str());
        logger::debug("Command name '%s' doesn't match name regex '[a-z][a-z-A-Z]+'", command.name.c_str());
        return;
    }

    if (commands.contains(command.name))
    {
        logger::warn("Command '%s' is already registered", command.name.c_str());
        return;
    }

    commands[command.name] = command;
}

void CommandsManager::addCommand(const std::string &name, Command::HandlerType handler, const std::string &usage, const std::string &description)
{
    addCommand({name,
                usage,
                description,
                handler});
}

CommandsManager::CallCommandError CommandsManager::callCommand(const ISender::SenderType type, ISender *sender, std::string commandString)
{
    if (type != ISender::SenderType::CONSOLE)
        throw std::runtime_error("Player and console logic not yet implemented");

    // Remove trailing spaces
    commandString = std::regex_replace(commandString, std::regex(" +$"), "");
    commandString += " ";

    size_t pos, startPos = 0;
    std::vector<std::string> splittedString;
    while ((pos = commandString.find(" ", startPos)) != std::string::npos)
    {
        std::string split = commandString.substr(startPos, pos - startPos);
        if (split.empty() || split == " ")
            return CallCommandError::FORMAT;

        splittedString.push_back(split);
        startPos = pos + 1;
    }

    std::string name = splittedString[0];
    std::vector<std::string> args(splittedString.begin() + 1, splittedString.end());

    if (!std::regex_match(name, nameRegex))
        return CommandsManager::FORMAT;

    if (!commands.contains(name))
        return CommandsManager::COMMAND_NOT_FOUND;

    Command &cmd = commands[name];
    try
    {
        cmd.handler(type, *sender, args);
    }
    catch (const std::exception &err)
    {
        logger::error("Command '%s' errored : %s", cmd.name.c_str(), err.what());
        logger::debug("Command context : command (%s), sender type (%d)", commandString.c_str(), (int)type);
        return CommandsManager::RUNTIME_ERROR;
    }

    return CommandsManager::NONE;
}