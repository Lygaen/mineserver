#include "serverlist.h"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <utils/config.h>
#include <utils/logger.h>

void ServerListPacket::write(IMCStream *stream)
{
    rapidjson::Document document;
    document.SetObject();

    auto &alloc = document.GetAllocator();

    rapidjson::Value version(rapidjson::kObjectType);
    version.AddMember("name", MC_VERSION_NAME, alloc);
    version.AddMember("protocol", MC_VERSION_NUMBER, alloc);
    document.AddMember("version", version, alloc);

    rapidjson::Value players(rapidjson::kObjectType);
    players.AddMember("max", Config::inst()->MAX_PLAYERS.getValue(), alloc);
    // TODO Display actual number of connected players with sample
    players.AddMember("online", 0, alloc);
    // players.AddMember("sample", rapidjson::Value(rapidjson::kArrayType), alloc);
    document.AddMember("players", players, alloc);

    rapidjson::Value description(rapidjson::kObjectType);
    Config::inst()->MOTD.getValue().save(description, alloc);
    document.AddMember("description", description, alloc);

    rapidjson::Value favicon(rapidjson::kStringType);

    const std::string &data = Config::inst()->ICON_FILE.getValue().getBase64String();
    std::string f = std::string("data:image/png;base64,") + data;
    favicon.SetString(f.c_str(), f.size());

    document.AddMember("favicon", favicon, alloc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    stream->writeString(std::string(buffer.GetString(), buffer.GetSize()));
}

void ServerListPacket::read(IMCStream *stream)
{
    /* Nothing wrong if you call it but just unecessary bloat */
}
