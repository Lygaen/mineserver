/**
 * @file pingpong.cpp
 * @author Lygaen
 * @brief The file containing Ping Pong Packet logic
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "pingpong.h"

void PingPongPacket::write(IMCStream *stream)
{
    stream->writeLong(payload);
}

void PingPongPacket::read(IMCStream *stream)
{
    payload = stream->readLong();
}

void PingPongPacket::loadLua(lua_State *state, const char *baseNamespaceName) {
    luabridge::getGlobalNamespace(state)
            .beginNamespace(baseNamespaceName)
            .beginClass<PingPongPacket>("PingPong")
            .addConstructor<void()>()
            .addProperty("payload", &PingPongPacket::payload)
            .endClass()
            .endNamespace();
}
