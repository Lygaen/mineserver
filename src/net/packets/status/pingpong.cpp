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
