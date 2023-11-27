#include <gtest/gtest.h>
#include <plugins/event.hpp>

class FakeEvent : public IEvent
{
public:
    int amount = 0;
};

void testEvent(FakeEvent &e)
{
    e.amount += 1;
}
TEST(Events, OneByOne)
{
    EventsManager events;
    FakeEvent e;

    auto subId = events.subscribe<FakeEvent>([](FakeEvent &e)
                                             { e.amount += 1; });
    events.fire(e);
    ASSERT_EQ(e.amount, 1);
    events.unsubscribe<FakeEvent>(subId);

    subId = events.subscribe<FakeEvent>(testEvent);
    events.fire(e);
    ASSERT_EQ(e.amount, 2);
    events.unsubscribe<FakeEvent>(subId);
}

TEST(Events, Multiple)
{
    EventsManager events;
    FakeEvent e;

    auto subId1 = events.subscribe<FakeEvent>([](FakeEvent &e)
                                              { e.amount += 1; });
    auto subId2 = events.subscribe<FakeEvent>(testEvent);
    events.fire(e);
    ASSERT_EQ(e.amount, 2);

    events.unsubscribe<FakeEvent>(subId1);
    events.unsubscribe<FakeEvent>(subId2);

    events.fire(e);
    ASSERT_EQ(e.amount, 2);
}

TEST(Events, RValueSubs)
{
    EventsManager events;
    FakeEvent e;

    auto subId = events.subscribe<FakeEvent>([](FakeEvent &e)
                                             { e.amount += 1; });
    events.fire(e);
    ASSERT_EQ(e.amount, 1);
    events.unsubscribe<FakeEvent>(subId);

    events.fire(e);
    ASSERT_EQ(e.amount, 1);
}

TEST(Events, FunctionSubs)
{
    EventsManager events;
    FakeEvent e;

    auto subId = events.subscribe<FakeEvent>(testEvent);
    events.fire(e);
    ASSERT_EQ(e.amount, 1);
    events.unsubscribe<FakeEvent>(subId);

    events.fire(e);
    ASSERT_EQ(e.amount, 1);
}