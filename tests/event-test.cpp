#include <gtest/gtest.h>
#include <event/event.h>

class FakeEvent : public IEvent
{
};

TEST(Events, FireLambda)
{
    EventHandler<FakeEvent> handler;

    bool eventFired = false;

    handler.subscribe([&eventFired](FakeEvent event)
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

    auto id = handler.subscribe([&eventFired](FakeEvent event)
                                { (void)event;
        eventFired = true; });

    handler.unsubscribe(id);

    FakeEvent e;
    handler.fire(e);

    ASSERT_FALSE(eventFired);
}
static bool eventFired = false;
void onFakeEvent(FakeEvent e)
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
}

TEST(Events, NotFireFunction)
{
    EventHandler<FakeEvent> handler;

    auto id = handler.subscribe(onFakeEvent);
    handler.unsubscribe(id);

    FakeEvent e;
    handler.fire(e);

    ASSERT_TRUE(eventFired);
}