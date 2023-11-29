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
