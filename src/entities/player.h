/**
 * @file player.h
 * @author Lygaen
 * @brief The file for handling player logic
 * @version 0.1
 * @date 2023-12-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_PLAYER_H
#define MINESERVER_PLAYER_H

#include <entities/entity.h>
#include <cmd/commands.h>

/**
 * @brief Player object
 *
 * Represents a player on the server,
 * as a living entity.
 */
class Player : public ILiving, ISender
{
public:
    /**
     * @brief Construct a new Player object
     *
     */
    Player() = default;
    /**
     * @brief Destroy the Player object
     *
     */
    ~Player() override = default;

    /**
     * @brief Name of the player
     *
     * Also called the username or tag
     */
    std::string name;

    /**
     * @brief Sends a message to a player
     *
     * @param message the message to send
     * @todo actually implement it
     */
    void sendMessage(const ChatMessage &message) override
    {
        throw std::runtime_error("Cannot send message to player !");
    }

    /**
     * @brief Loads the Player class to a Lua one
     *
     * @param state the state to load to
     * @param namespaceName the namespace to load to
     */
    static void loadLua(lua_State *state, const char *namespaceName)
    {
        luabridge::getGlobalNamespace(state)
            .beginNamespace(namespaceName)
            .beginClass<Player>("Player")
            .addProperty("name", &Player::name)
            .endClass()
            .endNamespace();
    }
};

#endif // MINESERVER_PLAYER_H