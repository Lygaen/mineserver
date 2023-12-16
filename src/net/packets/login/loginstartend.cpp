/**
 * @file loginstartend.cpp
 * @author Lygaen
 * @brief The file containing Login Start / Success packets logic
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "loginstartend.h"

void LoginStart::write(IMCStream *stream)
{
    (void)stream;
    // Does nothing
    throw std::runtime_error("LoginStart write should not be called !");
}

void LoginStart::read(IMCStream *stream)
{
    name = stream->readString();
}

void LoginStart::loadLua(lua_State *state, const char *baseNamespaceName)
{
    luabridge::getGlobalNamespace(state)
        .beginNamespace(baseNamespaceName)
        .beginClass<LoginStart>("LoginStart")
        .addConstructor<void()>()
        .endClass()
        .endNamespace();
}

void LoginSuccess::write(IMCStream *stream)
{
    // For this packet only it is written with hyphens
    stream->writeString(uuid.getFull());
    stream->writeString(username);
}

void LoginSuccess::read(IMCStream *stream)
{
    (void)stream;
    // Does nothing
    throw std::runtime_error("LoginStart read should not be called !");
}

void LoginSuccess::loadLua(lua_State *state, const char *baseNamespaceName)
{
    luabridge::getGlobalNamespace(state)
        .beginNamespace(baseNamespaceName)
        .beginClass<LoginSuccess>("LoginSuccess")
        .addConstructor<void(const std::string &, const UUID &)>()
        .endClass()
        .endNamespace();
}
