#include <gtest/gtest.h>
#include <event/event.h>

class FakeEvent : public IEvent
{
};

TEST(Events, FireLambda)
{
    EventHandler<FakeEvent> handler;

    bool eventFired = false;

    handler.subscribe([&eventFired](FakeEvent &event)
                      { (void)event;
        eventFired = true; });

    FakeEvent e;
    handler.fire(e);

    ASSERT_TRUE(eventFired);
}

TEST(Events, NotFireLambda)
{
    EventHandler<FakeEvent> handler;

    bool eventFired = false;

    auto id = handler.subscribe([&eventFired](FakeEvent &event)
                                { (void)event;
        eventFired = true; });

    handler.unsubscribe(id);

    FakeEvent e;
    handler.fire(e);

    ASSERT_FALSE(eventFired);
}

static bool eventFired = false;
void onFakeEvent(FakeEvent &e)
{
    eventFired = true;
}

TEST(Events, FireFunction)
{
    EventHandler<FakeEvent> handler;

    handler.subscribe(onFakeEvent);

    FakeEvent e;
    handler.fire(e);

    ASSERT_TRUE(eventFired);
    eventFired = false;
}

TEST(Events, NotFireFunction)
{
    EventHandler<FakeEvent> handler;

    auto id = handler.subscribe(onFakeEvent);
    handler.unsubscribe(id);

    FakeEvent e;
    handler.fire(e);

    ASSERT_FALSE(eventFired);
    eventFired = false;
}

class FakeEvent2 : public IEvent
{
};

TEST(Events, ManagerSingle)
{
    EventsManager manager;

    bool eventFired = false;
    EventsManager::inst()->subscribe<FakeEvent>([&eventFired](FakeEvent &e)
                                                { eventFired = true; });
    FakeEvent e;
    EventsManager::inst()->fire(e);

    ASSERT_TRUE(eventFired);
}

TEST(Events, ManagerTwo)
{
    EventsManager manager;

    int eventFired = 0;
    EventsManager::inst()->subscribe<FakeEvent>([&eventFired](FakeEvent &e)
                                                { eventFired++; });
    EventsManager::inst()->subscribe<FakeEvent2>([&eventFired](FakeEvent2 &e)
                                                 { eventFired++; });
    FakeEvent e;
    EventsManager::inst()->fire(e);
    FakeEvent2 e2;
    EventsManager::inst()->fire(e2);

    ASSERT_EQ(eventFired, 2);
}

TEST(Events, ManagerNotFire)
{
    EventsManager manager;

    bool eventFired = false;
    EventsManager::inst()->subscribe<FakeEvent>([&eventFired](FakeEvent &e)
                                                { eventFired = true; });

    FakeEvent2 e2;
    EventsManager::inst()->fire(e2);

    ASSERT_FALSE(eventFired);
}