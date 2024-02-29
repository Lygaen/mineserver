/**
 * @file handshake.cpp
 * @author Lygaen
 * @brief The file containing handshake logic
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "handshake.h"
#include <stdexcept>
#include <utils/config.h>

void HandshakePacket::write(IMCStream *stream)
{
    (void)stream;
    // Does nothing
    throw std::runtime_error("Handshake write should not be called !");
}

void HandshakePacket::read(IMCStream *stream)
{
    protocolVersion = stream->readVarInt();
    serverAddress = stream->readString();
    serverPort = stream->readUnsignedShort();
    nextState = static_cast<ClientState>(stream->readVarInt());
}

void HandshakePacket::loadLua(lua_State *state, const char *baseNamespaceName) {
    luabridge::getGlobalNamespace(state)
            .beginNamespace(baseNamespaceName)
            .beginClass<HandshakePacket>("Handshake")
            .addConstructor<void()>()
            .addProperty("protocolVersion", &HandshakePacket::protocolVersion)
            .addProperty("serverAddress", &HandshakePacket::serverAddress)
            .addProperty("serverPort", &HandshakePacket::serverPort)
            .addProperty("nextState", &HandshakePacket::nextState)
            .endClass()
            .endNamespace();
}
