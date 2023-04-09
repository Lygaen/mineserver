#include <gtest/gtest.h>
#include <net/types/chatmessage.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

TEST(Types, ChatMessage)
{
    ChatMessage msg("This is some weird player message (they strike again!)");
    msg.bold = true;
    msg.strikethrough = true;
    msg.italic = true;
    msg.obfuscated = true;
    msg.underlined = true;

    rapidjson::Document doc(rapidjson::kObjectType);
    msg.save(doc, doc.GetAllocator());

    ChatMessage msg2;
    msg2.load(doc);

    ASSERT_EQ(msg, msg2);
}