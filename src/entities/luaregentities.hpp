/**
 * @file luaregentities.hpp
 * @author Lygaen
 * @brief Utility for loading types lua classes
 * @version 0.1
 * @date 2024-01-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MINESERVER_LUAREGENTITIES_H
#define MINESERVER_LUAREGENTITIES_H

#include <plugins/luaheaders.h>
#include <entities/entity.h>
#include <entities/player.h>

/**
 * @brief Loads entities classes to lua
 *
 * @param state the lua state to load to
 */
void loadEntitiesLua(lua_State *state)
{
    const char *namespaceName = "entities";

    IEntity::loadLua(state, namespaceName);
    ILiving::loadLua(state, namespaceName);
    Player::loadLua(state, namespaceName);
}

#endif // MINESERVER_LUAREGENTITIES_H