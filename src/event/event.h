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
    using callbackType = std::function<void(T &)>;
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

    /**
     * @brief Get the next Id
     *
     * Utility for generating ids for subscription system
     * @return subId the unique id
     */
    subId getNextId() { return ++nextId; }

public:
    /**
     * @brief Construct a new Event Handler object
     *
     * It will not compile if you try it with a
     * class that does not derive from #IEvent
     */
    EventHandler();

    /**
     * @brief Fire an event
     *
     * Cascade the event to all of the listeners
     * @param event the event
     */
    void fire(T &event);
    /**
     * @brief Subscribe to an event
     *
     * Subscribe to an event for lvalue functions
     * @param func the function that will listen to events
     * @return subId the id to use for #unsubscribe
     */
    subId subscribe(const callbackType &func);
    /**
     * @brief Subscribe to an event
     *
     * Subscribe to an event for rvalue functions
     * @param func the function that will listen to events
     * @return subId the id to use for #unsubscribe
     */
    subId subscribe(callbackType &&func);
    /**
     * @brief Unsubscribe from an event
     *
     * Stop listening from events
     * @param id the id of the listener
     */
    void unsubscribe(subId id);
};

template <class T>
inline EventHandler<T>::EventHandler() : subs()
{
    static_assert(std::is_base_of_v<IEvent, T>, "Class doesn't derive from IEvent");
}

template <class T>
inline void EventHandler<T>::fire(T &event)
{
    for (auto &sub : subs)
    {
        sub.callback(event);
    }
}

template <class T>
inline EventHandler<T>::subId EventHandler<T>::subscribe(const callbackType &func)
{
    subscription s;
    s.callback = static_cast<callbackType>(func);
    s.id = getNextId();
    subs.push_back(s);
    return s.id;
}

template <class T>
inline EventHandler<T>::subId EventHandler<T>::subscribe(callbackType &&func)
{
    subscription s;
    s.callback = std::move(func);
    s.id = getNextId();
    subs.push_back(s);
    return s.id;
}

template <class T>
inline void EventHandler<T>::unsubscribe(subId id)
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

#endif // MINESERVER_EVENT_H