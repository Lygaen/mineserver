#ifndef MINESERVER_EVENT_H
#define MINESERVER_EVENT_H

#include <functional>
#include <type_traits>

/**
 * @brief Interface for all events
 *
 * Really for sanity in the end.
 */
class IEvent
{
};

/**
 * @brief Handler for a type of event
 *
 * @todo Make it thread-safe
 * @tparam T the event that should extend IEvent
 */
template <class T>
class EventHandler
{
public:
    /**
     * @brief The type of function that should be passed
     *
     * All event-receiving functions should be declared
     * as a void return type with a single parameter,
     * the event in question.
     */
    typedef std::function<void(T &)> callbackType;
    /**
     * @brief The Subscription Id
     *
     * Used to unsubscribe from events.
     */
    typedef int subId;

private:
    struct subscription
    {
        callbackType callback;
        subId id;
    };

    std::vector<subscription> subs;
    subId nextId = 0;
    const std::type_info *typeInfo;

    /**
     * @brief Get the next Id
     *
     * Utility for generating ids for subscription system
     * @return subId the unique id
     */
    subId
    getNextId()
    {
        return ++nextId;
    }

public:
    /**
     * @brief Construct a new Event Handler object
     *
     * It will not compile if you try it with a
     * class that does not derive from #IEvent
     */
    EventHandler() : subs()
    {
        static_assert(std::is_base_of_v<IEvent, T>, "Class doesn't derive from IEvent");
        typeInfo = &typeid(T);
    }

    /**
     * @brief Fire an event
     *
     * Cascade the event to all of the listeners
     * @param event the event
     */
    void fire(T &event)
    {
        for (auto &sub : subs)
        {
            sub.callback(event);
        }
    }

    /**
     * @brief Subscribe to an event
     *
     * Subscribe to an event for lvalue functions
     * @param func the function that will listen to events
     * @return subId the id to use for #unsubscribe
     */
    subId subscribe(const callbackType &func)
    {
        subscription s;
        s.callback = static_cast<callbackType>(func);
        s.id = getNextId();
        subs.push_back(s);
        return s.id;
    }

    /**
     * @brief Subscribe to an event
     *
     * Subscribe to an event for rvalue functions
     * @param func the function that will listen to events
     * @return subId the id to use for #unsubscribe
     */
    subId subscribe(callbackType &&func)
    {
        subscription s;
        s.callback = std::move(func);
        s.id = getNextId();
        subs.push_back(s);
        return s.id;
    }

    /**
     * @brief Unsubscribe from an event
     *
     * Stop listening from events
     * @param id the id of the listener
     */
    void unsubscribe(subId id)
    {
        subs.erase(
            std::remove_if(
                subs.begin(),
                subs.end(),
                [id](subscription el) -> bool
                {
                    return el.id == id;
                }),
            subs.end());
    }

    const std::type_info *getTypeInfo() const { return typeInfo; }
};

class EventsManager
{
private:
    static EventsManager *INSTANCE;
    std::vector<EventHandler<void *> *> handlers;

    template <class T>
    EventHandler<T> *getOrNull()
    {
        auto loc = std::find_if(handlers.begin(), handlers.end(), [](const EventHandler<void *> *entry)
                                { return *entry->getTypeInfo() == typeid(T); });

        if (loc == handlers.end())
            return nullptr;

        return reinterpret_cast<EventHandler<T> *>(*loc);
    }

    template <class T>
    EventHandler<T> *getOrCreateHandler()
    {
        auto handler = getOrNull<T>();

        if (!handler)
        {
            handler = new EventHandler<T>();
            handlers.push_back(reinterpret_cast<EventHandler<void *> *>(handler));
        }

        return handler;
    }

public:
    EventsManager() : handlers()
    {
        INSTANCE = this;
    }

    ~EventsManager()
    {
        auto loc = handlers.begin();
        while (loc != handlers.end())
        {
            delete *loc;
            loc = handlers.erase(loc);
        }
    }

    template <class T>
    EventHandler<T>::subId subscribe(const EventHandler<T>::callbackType &callback)
    {
        static_assert(std::is_base_of_v<IEvent, T>, "Class doesn't derive from IEvent");
        EventHandler<T> *handler = getOrCreateHandler<T>();
        return handler->subscribe(callback);
    }

    template <class T>
    EventHandler<T>::subId subscribe(EventHandler<T>::callbackType &&callback)
    {
        static_assert(std::is_base_of_v<IEvent, T>, "Class doesn't derive from IEvent");
        EventHandler<T> *handler = getOrCreateHandler<T>();
        return handler->subscribe(std::move(callback));
    }

    template <class T>
    void subscribe(EventHandler<T>::subId subId)
    {
        static_assert(std::is_base_of_v<IEvent, T>, "Class doesn't derive from IEvent");
        EventHandler<T> *handler = getOrCreateHandler<T>();
        return handler->unsubscribe(subId);
    }

    template <class T>
    void fire(T &event)
    {
        static_assert(std::is_base_of_v<IEvent, T>, "Class doesn't derive from IEvent");
        EventHandler<T> *handler = getOrNull<T>();

        if (handler)
        {
            handler->fire(event);
        }
    }

    static EventsManager *inst()
    {
        return INSTANCE;
    }
};
EventsManager *EventsManager::INSTANCE = nullptr;

#endif // MINESERVER_EVENT_H