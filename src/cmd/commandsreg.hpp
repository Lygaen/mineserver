/**
 * @file commandsreg.hpp
 * @author Lygaen
 * @brief The file containing most commands registering
 * @version 0.1
 * @date 2024-02-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MINESERVER_COMMANDSREG_H
#define MINESERVER_COMMANDSREG_H

#include <cmd/commands.h>
#include <utils/config.h>
#include <plugins/plugins.h>
#include <server.h>

/**
 * @brief Handler for help message
 *
 * @param senderType sender type
 * @param sender the actual sender
 * @param args all of the args
 */
void helpMessage(const ISender::SenderType senderType, ISender &sender, const std::vector<std::string> &args)
{
    if (args.size() != 0)
    {
        sender.sendMessage(ChatMessage("/help doesn't accept any arguments !"));
    }

    auto mapCommands = CommandsManager::inst().getCommands();
    std::string finalString;

    for (auto &kv : mapCommands)
    {
        Command &command = kv.second;
        finalString += "/" + command.name + " " + command.usage + "\n";
        finalString += command.description + "\n";
    }

    sender.sendMessage(finalString);
}

/**
 * @brief Handler for plugins message
 *
 * @param senderType sender type
 * @param sender the actual sender
 * @param args all of the args
 */
void pluginsMessage(const ISender::SenderType senderType, ISender &sender, const std::vector<std::string> &args)
{
    if (args.size() != 0)
    {
        sender.sendMessage(ChatMessage("/plugins doesn't accept any arguments !"));
        return;
    }

    auto plugins = PluginsManager::inst().getPlugins();
    if (plugins.size() <= 0)
    {
        sender.sendMessage(ChatMessage("No plugins registered"));
        return;
    }

    std::string finalString;

    for (auto plugin : plugins)
    {
        finalString += plugin->name + " (v" + plugin->version + "), ";
    }

    finalString.pop_back();
    finalString.pop_back();

    finalString += " [" + std::to_string(plugins.size()) + "]";

    sender.sendMessage(finalString);
}

/**
 * @brief Register commands
 *
 */
void registerCommands()
{
    CommandsManager::inst().addCommand(
        "help", helpMessage,
        "", "Returns help message");

    CommandsManager::inst().addCommand(
        "stop", [](const ISender::SenderType, ISender &, const std::vector<std::string> &)
        { Server::inst()->stop(); },
        "", "Stops the server");

    CommandsManager::inst().addCommand(
        "plugins", pluginsMessage,
        "", "Shows a list of installed plugins");

    Config::inst()->registerCommands();
}

#endif // MINESERVER_COMMANDSREG_H