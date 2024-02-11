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

    clickEvent.load(document);

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
#undef WV

#define WS(x)          \
    if (x.size() != 0) \
    document.AddMember(#x, rapidjson::Value(x.c_str(), x.length(), alloc), alloc)

    WS(color);
    WS(insertion);
    WS(text);

#undef WS

    clickEvent.save(document, alloc);

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
        .addConstructor([](void *ptr)
                        { return new (ptr) ChatMessage(); },
                        [](void *ptr, const std::string &msg)
                        { return new (ptr) ChatMessage(msg); })
        .addProperty("bold", &ChatMessage::bold)
        .addProperty("italic", &ChatMessage::italic)
        .addProperty("underlined", &ChatMessage::underlined)
        .addProperty("strikethrough", &ChatMessage::strikethrough)
        .addProperty("obfuscated", &ChatMessage::obfuscated)
        .addProperty("color", &ChatMessage::color)
        .addProperty("insertion", &ChatMessage::insertion)
        .addProperty("text", &ChatMessage::text)
        .addFunction("addExtra", &ChatMessage::addExtra)
        .addProperty("clickEvent", &ChatMessage::clickEvent)
        .endClass()
        .endNamespace();

    ClickEvent::loadLua(state, namespaceName);
}

ChatMessage::ClickEvent::ClickEvent(ActionType action, const std::string &value)
    : action(action), value(value)
{
}

ChatMessage::ClickEvent::ClickEvent(uint changePage)
    : action(ActionType::CHANGE_PAGE), value(std::to_string(changePage))
{
}

ChatMessage::ClickEvent::ClickEvent() : action(ActionType::NONE)
{
}

const std::unordered_map<std::string, ChatMessage::ClickEvent::ActionType> ACTION_TABLE{
    {"open_url", ChatMessage::ClickEvent::OPEN_URL},
    {"run_command", ChatMessage::ClickEvent::RUN_COMMAND},
    {"suggest_command", ChatMessage::ClickEvent::SUGGEST_COMMAND},
    {"change_page", ChatMessage::ClickEvent::CHANGE_PAGE},
    {"copy_to_clipboard", ChatMessage::ClickEvent::COPY_TO_CLIPBOARD},
};

void ChatMessage::ClickEvent::load(const rapidjson::Value &document)
{
    rapidjson::Document::ConstMemberIterator it;
    if ((it = document.FindMember("clickEvent")) == document.MemberEnd() ||
        !it->value.IsObject())
        return;
    const rapidjson::Value &clickEvent = it->value;

    if ((it = clickEvent.FindMember("action")) == document.MemberEnd() ||
        !it->value.IsString())
        return;
    std::string stringAction(it->value.GetString(), it->value.GetStringLength());

    if ((it = clickEvent.FindMember("value")) == document.MemberEnd() ||
        !it->value.IsString())
        return;
    value = std::string(it->value.GetString(), it->value.GetStringLength());

    if (!ACTION_TABLE.contains(stringAction))
    {
        action = ActionType::NONE;
        value = "";
        return;
    }

    action = ACTION_TABLE.at(stringAction);
}

void ChatMessage::ClickEvent::save(rapidjson::Value &document, rapidjson::Document::AllocatorType &alloc) const
{
    if (action == ActionType::NONE)
        return;

    rapidjson::Value clickEvent(rapidjson::kObjectType);
    auto it = std::find_if(std::begin(ACTION_TABLE), std::end(ACTION_TABLE),
                           [this](auto &&p)
                           { return std::get<1>(p) == action; });
    if (it == std::end(ACTION_TABLE))
        throw std::runtime_error("Could not parse action !");

    std::string stringAction = it->first;

    clickEvent.AddMember("action", rapidjson::Value(stringAction.c_str(), stringAction.length(), alloc), alloc);
    clickEvent.AddMember("value", rapidjson::Value(value.c_str(), value.length(), alloc), alloc);

    document.AddMember("clickEvent", clickEvent, alloc);
}

void ChatMessage::ClickEvent::loadLua(lua_State *state, const char *namespaceName)
{
    luabridge::getGlobalNamespace(state)
        .beginNamespace(namespaceName)
        .beginNamespace("ChatMessage")
        .beginClass<ChatMessage::ClickEvent>("ClickEvent")
        .addProperty("action", &ClickEvent::action)
        .addProperty("value", &ClickEvent::value)
        .endClass()
        .endNamespace()
        .endNamespace();
}
