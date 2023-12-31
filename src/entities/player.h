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

/**
 * @brief Player object
 *
 * Represents a player on the server,
 * as a living entity.
 */
class Player : public ILiving
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
    ~Player() = default;

    /**
     * @brief Name of the player
     *
     * Also called the username or tag
     */
    std::string name;
    // TODO implement everything in Lua
};

#endif // MINESERVER_PLAYER_H