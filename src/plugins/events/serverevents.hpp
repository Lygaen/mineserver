/**
 * @file serverevents.hpp
 * @author Lygaen
 * @brief The file containing all of the server events
 * @version 0.1
 * @date 2023-12-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_SERVEREVENTS_H
#define MINESERVER_SERVEREVENTS_H

#include <plugins/event.h>

/**
 * @brief Event for a server starting
 *
 * Launched when the server starts,
 * in practice straight after the plugins
 * were loaded.
 * Should implement in lua as such :
 * @code{.lua}
 * local function onStart(e)
 *     -- do your thing
 * end
 *
 * event.onServerStart(onClient)
 * @endcode
 */
class ServerStartEvent : public IEvent<ServerStartEvent>
{
};

#endif // MINESERVER_SERVEREVENTS_H