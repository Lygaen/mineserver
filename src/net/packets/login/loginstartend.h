/**
 * @file loginstartend.h
 * @author Mathieu Cayeux
 * @brief The file containing login start and end packets
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_LOGINSTARTEND_H
#define MINESERVER_LOGINSTARTEND_H

#include <net/packet.h>
#include <plugins/luaheaders.h>
#include <net/types/uuid.h>

/**
 * @brief Login Start Packet
 *
 * Packet sent by the client containing
 * initial player name as well as a signal
 * to start encryption.
 */
class LoginStart : public IPacket
{
private:
    /**
     * @brief Write Packet Data
     *
     * @param stream the stream to write to
     * @deprecated should not be used, useless
     */
    void write(IMCStream *stream) override;

public:
    /**
     * @brief Construct a new Login Start object
     *
     */
    LoginStart() : IPacket(0x00) {}

    /**
     * @brief Name of the player
     *
     * Name of the player of the client connecting
     * to the server.
     */
    std::string name;

    /**
     * @brief Reads Packet data
     *
     * @param stream the stream to read from
     */
    void read(IMCStream *stream) override;

    /**
     * @brief Loads Packet to lua
     *
     * @param state lua state to load to
     * @param baseNamespaceName base namespace to register to
     */
    static void loadLua(lua_State *state, const char *baseNamespaceName);
};

/**
 * @brief Login Success Packet
 *
 * Should be sent by the server when everything
 * went according to plan when trying to log in
 * the client.
 */
class LoginSuccess : public IPacket
{
private:
    /**
     * @brief Writes Packet Data
     *
     * @param stream the stream to write to
     */
    void write(IMCStream *stream) override;

public:
    /**
     * @brief Construct a new Login Success object
     *
     * @param username the username given to the player
     * @param uuid the uuid of the player
     */
    LoginSuccess(const std::string &username, const UUID &uuid) : IPacket(0x02), username(username), uuid(uuid) {}

    /**
     * @brief Username of the player
     *
     * The username that is given by the server
     * to the client.
     */
    std::string username;
    /**
     * @brief UUID of the player
     *
     * The UUID given by the server to the client.
     */
    UUID uuid;

    /**
     * @brief Reads Packet Data
     *
     * @param stream the stream to read from
     * @deprecated should not be used, useless
     */
    void read(IMCStream *stream) override;

    /**
     * @brief Loads Packet to lua state
     *
     * @param state the lua state to load to
     * @param baseNamespaceName the base namespace to register to
     */
    static void loadLua(lua_State *state, const char *baseNamespaceName);
};

#endif // MINESERVER_LOGINSTARTEND_H