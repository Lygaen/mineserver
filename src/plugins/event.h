/**
 * @file event.hpp
 * @author Mathieu Cayeux
 * @brief The header-only containing event logic
 * @version 0.1
 * @date 2023-11-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_EVENT_HPP
#define MINESERVER_EVENT_HPP

#include <functional>
#include <type_traits>
#include <plugins/luaheaders.h>

/**
 * @brief Interface for all events
 *
 * Really for sanity in the end.
 */
template<class T>
class IEvent
{
public:
    static void loadLua(lua_State *state);
};

template <typename T>
constexpr auto type_name() {
    std::string_view name, prefix, suffix;
#ifdef __clang__
    name = __PRETTY_FUNCTION__;
  prefix = "auto type_name() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
    name = __PRETTY_FUNCTION__;
    prefix = "constexpr auto type_name() [with T = ";
    suffix = "]";
#elif defined(_MSC_VER)
    name = __FUNCSIG__;
  prefix = "auto __cdecl type_name<";
  suffix = ">(void)";
#endif
    name.remove_prefix(prefix.size());
    name.remove_suffix(suffix.size());
    return name;
}

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
        static_assert(std::is_base_of_v<IEvent<T>, T>, "Class doesn't derive from IEvent");
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
            try {
                sub.callback(event);
            } catch(const luabridge::LuaException& e) {
                logger::error("Could not launch event %s in lua", std::string(type_name<T>()).c_str());
            }
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

    /**
     * @brief Get the Type Info of T
     *
     * Returns the type info of T
     * @return const std::type_info* the type info of T
     */
    const std::type_info *getTypeInfo() const { return typeInfo; }
};

/**
 * @brief Manager for Events
 *
 */
class EventsManager
{
private:
    std::vector<EventHandler<void *> *> handlers;
    static EventsManager *INSTANCE;

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
    /**
     * @brief Construct a new Events Manager object
     *
     */
    EventsManager();

    /**
     * @brief Destroy the Events Manager object
     *
     */
    ~EventsManager()
    {
        auto loc = handlers.begin();
        while (loc != handlers.end())
        {
            delete *loc;
            loc = handlers.erase(loc);
        }
    }

    /**
     * @brief Subscribe to T event
     *
     * Subscribes function to the event
     * @tparam T the event to subscribe to
     * @param callback the function to subscribe
     * @return EventHandler<T>::subId the handler used for unsuscribing
     */
    template <class T>
    EventHandler<T>::subId subscribe(const EventHandler<T>::callbackType &callback)
    {
        static_assert(std::is_base_of_v<IEvent<T>, T>, "Class doesn't derive from IEvent");
        EventHandler<T> *handler = getOrCreateHandler<T>();
        return handler->subscribe(callback);
    }

    /**
     * @brief Subscribe to T event
     *
     * Subscribes an r-value function to the event
     * @tparam T the event to subscribe to
     * @param callback the r-value function to subscribe
     * @return EventHandler<T>::subId the handler used for unsuscribing
     */
    template <class T>
    EventHandler<T>::subId subscribe(EventHandler<T>::callbackType &&callback)
    {
        static_assert(std::is_base_of_v<IEvent<T>, T>, "Class doesn't derive from IEvent");
        EventHandler<T> *handler = getOrCreateHandler<T>();
        return handler->subscribe(std::move(callback));
    }

    /**
     * @brief Unsubscribe to T event
     *
     * Unsuscribe to the event
     * @tparam T the event to unsubscribe to
     * @param subId the handler of the function
     */
    template <class T>
    void unsubscribe(EventHandler<T>::subId subId)
    {
        static_assert(std::is_base_of_v<IEvent<T>, T>, "Class doesn't derive from IEvent");
        EventHandler<T> *handler = getOrCreateHandler<T>();
        return handler->unsubscribe(subId);
    }

    /**
     * @brief Fire T event
     *
     * Fire event for all functions that are subscribed to it
     * @tparam T the event
     * @param event the event value
     */
    template <class T>
    void fire(T &event)
    {
        static_assert(std::is_base_of_v<IEvent<T>, T>, "Class doesn't derive from IEvent");
        EventHandler<T> *handler = getOrNull<T>();

        if (handler)
        {
            handler->fire(event);
        }
    }

    static EventsManager *inst();
};

template<class T>
void IEvent<T>::loadLua(lua_State *state) {
    auto baseName = std::string(type_name<T>());
    std::string cleanedEvent = baseName.substr(0, baseName.length() - std::string("Event").length());
    static_assert(std::is_base_of_v<IEvent, T>, "Class doesn't derive from IEvent");
    luabridge::getGlobalNamespace(state)
            .beginNamespace("event")
            .addFunction(("on"+cleanedEvent).c_str(), [](const luabridge::LuaRef& ref) {
                if(!ref.isFunction())
                    return;

                EventsManager::inst()->subscribe<T>(const_cast<luabridge::LuaRef&>(ref));
            })
            .template beginClass<T>(cleanedEvent.c_str())
            .endClass()
            .endNamespace();
}

#endif // MINESERVER_EVENT_HPP