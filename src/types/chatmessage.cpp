/**
 * @file chatmessage.cpp
 * @author Lygaen
 * @brief The file containing Chat Message logic
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "chatmessage.h"

#include <utility>

ChatMessage::ChatMessage() = default;

ChatMessage::ChatMessage(const std::string &msg) : text(msg)
{
}

ChatMessage::ChatMessage(const std::string &&msg) : text(msg)
{
}

ChatMessage::~ChatMessage()
{
    if (next == nullptr)
        return;

    delete next;
}

void ChatMessage::addExtra(ChatMessage *cm)
{
    if (next == nullptr)
    {
        next = cm;
        return;
    }

    ChatMessage *n = next;
    while (n->next != nullptr)
    {
        n = n->next;
    }

    n->next = cm;
}

inline void writeSafely(std::string key, rapidjson::Value v, rapidjson::Value &document, rapidjson::Document::AllocatorType &alloc)
{
    document.AddMember(rapidjson::StringRef(key.data()), v, alloc);
}

void ChatMessage::load(const rapidjson::Value &document)
{
    if (!document.IsObject())
        return;
    rapidjson::Document::ConstMemberIterator it;

#define LB(x)                                                     \
    if ((it = document.FindMember(#x)) != document.MemberEnd() && \
        it->value.IsBool())                                       \
        x = it->value.GetBool();

    LB(bold);
    LB(italic);
    LB(underlined);
    LB(strikethrough);
    LB(obfuscated);

#define LS(x)                                                     \
    if ((it = document.FindMember(#x)) != document.MemberEnd() && \
        it->value.IsString())                                     \
        x = std::string(it->value.GetString(), it->value.GetStringLength());

    LS(color);
    LS(insertion);
    LS(text);

    if ((it = document.FindMember("extra")) != document.MemberEnd() &&
        it->value.IsArray())
    {
        ChatMessage *n = this;
        for (rapidjson::SizeType i = 0; i < it->value.Size(); i++)
        {
            ChatMessage *temp = new ChatMessage();
            n->addExtra(temp);
            n = temp;
            n->load(it->value[i]);
        }
    }
}

void ChatMessage::save(rapidjson::Value &document, rapidjson::Document::AllocatorType &alloc) const
{
#define WV(x) \
    if (x)    \
    document.AddMember(#x, rapidjson::Value(x), alloc)

    WV(bold);
    WV(italic);
    WV(underlined);
    WV(strikethrough);
    WV(obfuscated);

#define WS(x)          \
    if (x.size() != 0) \
    document.AddMember(#x, rapidjson::Value(x.c_str(), x.length(), alloc), alloc)

    WS(color);
    WS(insertion);
    WS(text);

    if (next == nullptr)
        return;
    ChatMessage *n = next;
    rapidjson::Value extra(rapidjson::kArrayType);
    do
    {
        rapidjson::Value temp(rapidjson::kObjectType);
        n->save(temp, alloc);
        extra.PushBack(temp, alloc);
    } while ((n = n->next) != nullptr);

    document.AddMember("extra", extra, alloc);
}

bool operator==(const ChatMessage &lhs, const ChatMessage &rhs)
{
#define CMP(x) lhs.x == rhs.x &&
    return CMP(text) CMP(bold) CMP(italic) CMP(underlined)
        CMP(strikethrough) CMP(obfuscated)
            CMP(color) CMP(insertion) true;
}

void ChatMessage::loadLua(lua_State *state, const char *namespaceName) {
    luabridge::getGlobalNamespace(state)
    .beginNamespace(namespaceName)
    .beginClass<ChatMessage>("ChatMessage")
            .addConstructor([](void* ptr) {return new (ptr) ChatMessage();},
                            [](void* ptr, const std::string &msg) {return new (ptr) ChatMessage(msg);})
            .addProperty("bold", &ChatMessage::bold)
            .addProperty("italic", &ChatMessage::italic)
            .addProperty("underlined", &ChatMessage::underlined)
            .addProperty("strikethrough", &ChatMessage::strikethrough)
            .addProperty("obfuscated", &ChatMessage::obfuscated)
            .addProperty("color", &ChatMessage::color)
            .addProperty("insertion", &ChatMessage::insertion)
            .addProperty("text", &ChatMessage::text)
            .addFunction("addExtra", &ChatMessage::addExtra)
    .endClass()
    .endNamespace();
}