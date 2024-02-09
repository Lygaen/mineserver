/**
 * @file commands.h
 * @author Lygaen
 * @brief The file handling commands logic
 * @version 0.1
 * @date 2024-02-09
 *
 * @copyright Copyright (c) 2024
 * @todo Register commands in Lua
 */

#ifndef MINESERVER_COMMANDS_H
#define MINESERVER_COMMANDS_H

#include <unordered_map>
#include <memory>
#include <functional>
#include <string>
#include <regex>

/**
 * @brief Sender Interface
 *
 * Interface for all command-sending
 * capable things (Players, command blocks
 * console).
 *
 * @todo Actually implement it lol
 */
class ISender
{
public:
    /**
     * @brief Sender type
     *
     * Type of the sender, for casting
     * utilities.
     */
    enum SenderType
    {
        CONSOLE = 0,
        PLAYER,
        COMMAND_BLOCK
    };
};

/**
 * @brief Command struct
 *
 * The structure that holds all
 * data that a command should have.
 */
struct Command
{
    /**
     * @brief The name of the command
     *
     * Not the actual name. But more
     * like the label of the command
     * like for the Ping command the
     * name is `ping` so that it can
     * be invoked with `/ping`
     * Should follow the following
     * regex : `[a-z][a-z-A-Z]+`
     */
    std::string name;
    /**
     * @brief The usage of the command
     *
     * So that for a command that can
     * be invoked as such :
     * /msg Player123 This is a message
     * The usage should be this :
     * `<Player:PlayerName> [quiet] <...message>`
     * The optional parameters being
     * indicated with `[param]` and
     * the mandatory ones with `<param>`.
     * A parameter that takes all the
     * remaining one should be listed
     * as `<...param>` or `[...param]`
     * if it is optional.
     * The type is indicated after the `:`.
     */
    std::string usage;
    /**
     * @brief The description of the command
     *
     * The description shown in the help
     * message of what it does.
     */
    std::string description;

    /**
     * @brief Function handler type
     *
     * The type for a function handling
     * a command.
     */
    typedef std::function<void(const ISender::SenderType, ISender &, const std::vector<std::string> &)> HandlerType;
    /**
     * @brief Command handler
     *
     * The function that will
     * handle the command.
     */
    HandlerType handler;
};

/**
 * @brief Commands Manager
 *
 * The manager class for handling
 * all commands logic.
 */
class CommandsManager
{
private:
    std::unordered_map<std::string, Command> commands;

    static CommandsManager *instance;

public:
    /**
     * @brief Construct a new Commands Manager object
     *
     */
    CommandsManager();
    /**
     * @brief Destroy the Commands Manager object
     *
     */
    ~CommandsManager();

    /**
     * @brief Adds a command
     *
     * Adds it by struct value.
     * @param command the command to add
     */
    void addCommand(const Command &command);
    /**
     * @brief Adds a command
     *
     * Adds it by struct fields values.
     * @param name see Command::name
     * @param handler Command::handler
     * @param usage Command::usage
     * @param description Command::description
     */
    void addCommand(const std::string &name,
                    Command::HandlerType handler,
                    const std::string &usage = "",
                    const std::string &description = "");

    /**
     * @brief Call command return code
     *
     */
    enum CallCommandError
    {
        /**
         * @brief No errors
         *
         */
        NONE,
        /**
         * @brief Errors while parsing the command
         *
         */
        FORMAT,
        /**
         * @brief Command was not found
         *
         */
        COMMAND_NOT_FOUND,
        /**
         * @brief Error while running the command
         *
         */
        RUNTIME_ERROR,
    };
    CallCommandError callCommand(const ISender::SenderType type, ISender *sender, std::string commandString);

    /**
     * @brief Gets Commands Manager instance
     *
     * @return CommandsManager& the instance
     */
    static CommandsManager &inst()
    {
        return *instance;
    }
};

#endif // MINESERVER_COMMANDS_H