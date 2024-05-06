#include <gtest/gtest.h>
#include <types/chatmessage.h>
#include <rapidjson/prettywriter.h>
#include <types/uuid.h>

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

TEST(Types, UUID)
{
    std::string realUUID = "5999de96-5ade-4a40-bac9-29f690620fcc";
    std::string trimmed = realUUID;
    trimmed.erase(std::remove(trimmed.begin(), trimmed.end(), '-'), trimmed.end());

    MinecraftUUID uuid = MinecraftUUID::fromHex(realUUID);
    std::string s(reinterpret_cast<const char *>(uuid.getBytes()), 16);

    ASSERT_EQ(uuid.getFull(), realUUID);

    ASSERT_EQ(uuid.getTrimmed(), trimmed);

    ASSERT_TRUE(uuid.operator==(MinecraftUUID::fromHex(realUUID)));

    MinecraftUUID ui2 = MinecraftUUID::fromBytes(uuid.getBytes());

    ASSERT_EQ(uuid.getFull(), ui2.getFull());
    ASSERT_EQ(uuid.getTrimmed(), ui2.getTrimmed());
}