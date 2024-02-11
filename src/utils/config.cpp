/**
 * @file config.cpp
 * @author Lygaen
 * @brief The file containing the config logic
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <cmd/commands.h>
#include <utils/logger.h>
#include "config.h"

template <typename T>
Field<T>::Field(const char *section, const char *key, T def) : section(section), key(key), value(def) {}
template <typename T>
Field<T>::~Field() = default;

template <typename T>
inline rapidjson::Document::ConstMemberIterator Field<T>::canSafelyRead(const rapidjson::Document &document)
{
    auto loc = document.FindMember(section);
    if (loc == document.MemberEnd())
        return document.MemberEnd();
    return !loc->value.IsObject() ? document.MemberEnd() : loc->value.FindMember(key);
}

template <typename T>
void Field<T>::writeSafely(rapidjson::Document &document, rapidjson::Value &v)
{
    if (document.HasMember(section))
    {
        document[section].AddMember(rapidjson::StringRef(key), v, document.GetAllocator());
    }
    else
    {
        rapidjson::Value sec;
        sec.SetObject();
        sec.AddMember(rapidjson::StringRef(key), v, document.GetAllocator());
        document.AddMember(rapidjson::StringRef(section), sec, document.GetAllocator());
    }
}

template <typename T>
void Field<T>::load(const rapidjson::Document &document)
{
    (void)document;
    static_assert(sizeof(T) != sizeof(T), "Use available specializations");
}

template <typename T>
void Field<T>::save(rapidjson::Document &document)
{
    (void)document;
    static_assert(sizeof(T) != sizeof(T), "Use available specializations");
}

template <>
void Field<int>::load(const rapidjson::Document &document)
{
    auto loc = canSafelyRead(document);
    if (loc == document.MemberEnd())
        return;

    if (!loc->value.IsInt())
        return;

    value = loc->value.GetInt();
}
template <>
void Field<int>::save(rapidjson::Document &document)
{
    rapidjson::Value v;
    v.SetInt(value);
    writeSafely(document, v);
}

template <>
void Field<ChatMessage>::load(const rapidjson::Document &document)
{
    auto loc = canSafelyRead(document);
    if (loc == document.MemberEnd())
        return;

    if (!loc->value.IsObject())
        return;

    value = ChatMessage();
    value.load(loc->value);
}
template <>
void Field<ChatMessage>::save(rapidjson::Document &document)
{
    rapidjson::Value v;
    v.SetObject();
    value.save(v, document.GetAllocator());
    writeSafely(document, v);
}

template <>
void Field<std::string>::load(const rapidjson::Document &document)
{
    auto loc = canSafelyRead(document);
    if (loc == document.MemberEnd())
        return;

    if (!loc->value.IsString())
        return;

    value = std::string(loc->value.GetString(), loc->value.GetStringLength());
}
template <>
void Field<std::string>::save(rapidjson::Document &document)
{
    rapidjson::Value v;
    v.SetString(value.c_str(), value.length(), document.GetAllocator());
    writeSafely(document, v);
}

template <>
void Field<PNGFile>::load(const rapidjson::Document &document)
{
    auto loc = canSafelyRead(document);
    if (loc == document.MemberEnd())
        return;

    if (!loc->value.IsString())
        return;

    std::string s = std::string(std::string(loc->value.GetString(), loc->value.GetStringLength()));
    if (!s.empty())
        value = PNGFile(s);
}
template <>
void Field<PNGFile>::save(rapidjson::Document &document)
{
    rapidjson::Value v;
    v.SetString(value.getPath().c_str(), value.getPath().length(), document.GetAllocator());
    writeSafely(document, v);
}

template <>
void Field<bool>::load(const rapidjson::Document &document)
{
    auto loc = canSafelyRead(document);
    if (loc == document.MemberEnd())
        return;

    if (!loc->value.IsBool())
        return;

    value = loc->value.GetBool();
}
template <>
void Field<bool>::save(rapidjson::Document &document)
{
    rapidjson::Value v;
    v.SetBool(value);
    writeSafely(document, v);
}

template <typename T>
void Field<T>::registerLuaProperty(lua_State *state)
{
    luabridge::getGlobalNamespace(state)
        .beginNamespace("config")
        .beginNamespace(section)
        .addProperty(
            key, [this]()
            { return this->getValue(); },
            [this](T value)
            {
                this->setValue(value);
            })
        .endNamespace()
        .endNamespace();
}

constexpr const char *CONFIG_FILE = "config.json";
Config *Config::INSTANCE = nullptr;

Config::Config()
{
    if (INSTANCE)
        return;
    INSTANCE = this;

    if (!std::filesystem::exists(CONFIG_FILE))
    {
        std::ofstream out(CONFIG_FILE);
        out.close();

        logger::warn("Did not find config, creating default !");
    }

    load();
    save();
};
Config::~Config()
{
    INSTANCE = nullptr;
}

void Config::load()
{
    std::ifstream file(CONFIG_FILE);
    std::stringstream stout;
    stout << file.rdbuf();
    file.close();

    std::string content = stout.str();
    rapidjson::Document document;
    document.Parse(content.c_str());

    if (!document.IsObject())
        return;

#define UF(x) x.load(document);
    CONFIG_FIELDS
#undef UF

    logger::loadConfig();
    logger::debug("Loaded Config");
}

template <typename T>
bool printFieldValue(ISender &sender, const std::string &section, const std::string &key, Field<T> &field)
{
    if (section != field.section || key != field.key)
        return false;

    rapidjson::Document doc;
    doc.SetObject();
    field.save(doc);

    rapidjson::StringBuffer buff;
    buff.Clear();

    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
    doc.FindMember(section.c_str())->value.FindMember(key.c_str())->value.Accept(writer);

    sender.sendMessage(section + "." + key + ": " + std::string(buff.GetString(), buff.GetSize()));
    return true;
}

void handleConfigCommand(const ISender::SenderType senderType, ISender &sender, const std::vector<std::string> &args)
{
    if (args.size() == 0)
    {
        sender.sendMessage(ChatMessage("Invalid number of arguments"));
        return;
    }

    if (args[0] == "reload")
    {
        if (args.size() > 1)
        {
            sender.sendMessage(ChatMessage("Invalid number of arguments"));
            return;
        }

        Config::inst()->load();
        Config::inst()->save();
        sender.sendMessage(ChatMessage("Reloaded config"));
        return;
    }

    if (args[0] == "save")
    {
        if (args.size() > 1)
        {
            sender.sendMessage(ChatMessage("Invalid number of arguments"));
            return;
        }

        Config::inst()->save();
        sender.sendMessage(ChatMessage("Saved config"));
        return;
    }

    std::string fullField = args[0];
    if (fullField.find('.') == std::string::npos)
    {
        sender.sendMessage(ChatMessage("Field value doesn't match section.key"));
        return;
    }
    std::stringstream ffss(fullField);
    std::string section, key, seg;
    // Last expression to check if there are more than one
    if (!std::getline(ffss, section, '.') || !std::getline(ffss, key, '.') || std::getline(ffss, seg, '.'))
    {
        sender.sendMessage(ChatMessage("Field value doesn't match section.key"));
        return;
    }

    if (args.size() == 1)
    {
        bool wasFound = false;
#define UF(x)      \
    if (!wasFound) \
        wasFound = printFieldValue(sender, section, key, Config::inst()->x);
        CONFIG_FIELDS
#undef UF

        if (wasFound)
            return;

        sender.sendMessage("Config value at " + section + "." + key + " was not found");
        return;
    }

    std::string fullValue;
    bool skip = true;
    for (auto &arg : args)
    {
        if (skip) //  Skip first element
        {
            skip = false;
            continue;
        }
        fullValue += arg;
    }

    std::string jsonValue;
    jsonValue += "{";
    jsonValue += "\"" + section + "\":{";
    jsonValue += "\"" + key + "\":" + fullValue;
    jsonValue += "}}";

    rapidjson::Document doc;
    doc.Parse(jsonValue.c_str());

    if (doc.HasParseError() || !doc.IsObject())
    {
        sender.sendMessage(ChatMessage("Value is in invalid format !"));
        return;
    }

#define UF(x) Config::inst()->x.load(doc);
    CONFIG_FIELDS
#undef UF

    sender.sendMessage(ChatMessage("Set config value correctly !"));
}

void Config::registerCommands()
{
    CommandsManager::inst().addCommand(
        "config", handleConfigCommand,
        "<reload|save|field:Field> [newValue]", "Command to handle config things\nField type should be as such : section.key");
}

void Config::loadLuaLib(lua_State *state)
{
#define UF(x) x.registerLuaProperty(state);
    CONFIG_FIELDS
#undef UF
}

void Config::save()
{
    rapidjson::Document document;
    document.SetObject();

#define UF(x) x.save(document);
    CONFIG_FIELDS
#undef UF

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    std::ofstream file(CONFIG_FILE);
    file << buffer.GetString();
    file.close();
    logger::debug("Saved Config");
}
