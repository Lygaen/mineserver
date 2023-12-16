/**
 * @file event.cpp
 * @author Lygaen
 * @brief The file containing the event system logic
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "event.h"

EventsManager *EventsManager::INSTANCE;
EventsManager::EventsManager()
{
    INSTANCE = this;
}

EventsManager *EventsManager::inst()
{
    return INSTANCE;
}
