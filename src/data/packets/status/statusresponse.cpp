#include "statusresponse.h"
#include <utils/config.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <utils/logger.h>

void StatusResponse::read(StreamWrapper &wrapper)
{
}

void StatusResponse::write(StreamWrapper &wrapper)
{
    rapidjson::Document d;
    d.SetObject();

    auto &alloc = d.GetAllocator();

    rapidjson::Value version(rapidjson::kObjectType);
    version.AddMember("name", MINESERVER_VERSION_NAME, alloc);
    version.AddMember("protocol", MINESERVER_PROTOCOL_VERSION, alloc);
    d.AddMember("version", version, alloc);

    rapidjson::Value players(rapidjson::kObjectType);
    players.AddMember("max", Config::getInstance()->maxPlayers.get(), alloc);
    players.AddMember("online", 0, alloc); // TODO Change to actually count online players
    rapidjson::Value sample(rapidjson::kArrayType);
    players.AddMember("sample", sample, alloc);
    d.AddMember("players", players, alloc);

    rapidjson::Value description(rapidjson::kObjectType);
    description.AddMember("text", rapidjson::StringRef(Config::getInstance()->motd.get().c_str()), alloc);
    d.AddMember("description", description, alloc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    std::string s(buffer.GetString(), buffer.GetSize());

    wrapper.writeString(s);
}
