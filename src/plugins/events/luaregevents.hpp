/**
 * @file luaregevents.hpp
 * @author Lygaen
 * @brief Utility for loading events lua classes
 * @version 0.1
 * @date 2023-12-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_LUAREGEVENTS_H
#define MINESERVER_LUAREGEVENTS_H

#include <plugins/luaheaders.h>
#include <plugins/events/clientevents.hpp>
#include <plugins/events/serverevents.hpp>

/**
 * @brief Loads all events in lua
 *
 * @param state the lua state to load to
 */
void loadEventsLua(lua_State *state)
{
    ClientConnectedEvent::loadLua(state);
    ClientStatusEvent::loadLua(state);
    ServerStartEvent::loadLua(state);
}

#endif // MINESERVER_LUAREGEVENTS_H