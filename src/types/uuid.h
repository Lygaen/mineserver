/**
 * @file uuid.h
 * @author Lygaen
 * @brief The file containing MinecraftUUID logic
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_UUID_H
#define MINESERVER_UUID_H

#include <cstdint>
#include <string>
#include <array>
#include <plugins/luaheaders.h>

/**
 * @brief MinecraftUUID compliant data holder
 *
 * Data holder for an [RFC 4122](https://www.rfc-editor.org/rfc/pdfrfc/rfc4122.txt.pdf)
 * compliant MinecraftUUID.
 */
class MinecraftUUID
{
private:
    std::array<std::byte, 16> bytes;
    MinecraftUUID(const std::array<std::byte, 16> &bytes) : bytes(bytes) {}

public:
    /**
     * @brief Construct a new MinecraftUUID object
     *
     * In reality, just a wrapper around ::newRandom()
     */
    MinecraftUUID();

    /**
     * @brief Equality operator
     *
     * @param other the other member
     * @return true the members are equal
     * @return false the member are not equal
     */
    bool operator==(MinecraftUUID const &other);

    /**
     * @brief Get the hex of the MinecraftUUID with hyphens
     *
     * It will resemble something such as :
     * @code
     * 5999de96-5ade-4a40-bac9-29f690620fcc
     * @endcode
     * @return std::string the hex with hyphens
     */
    std::string getFull() const;
    /**
     * @brief Get the hex of the MinecraftUUID without hyphens
     *
     * It will resemble something such as :
     * @code
     * 5999de965ade4a40bac929f690620fcc
     * @endcode
     * @return std::string the hex without hyphens
     */
    std::string getTrimmed() const;
    /**
     * @brief Get the raw bytes of the MinecraftUUID
     *
     * @return const std::byte* pointer to byte data
     */
    const std::byte *getBytes() const;

    /**
     * @brief Constructs a new random MinecraftUUID
     *
     * @return MinecraftUUID the newly created MinecraftUUID
     */
    static MinecraftUUID newRandom();
    /**
     * @brief Constructs an MinecraftUUID from an username
     *
     * In reality, uses MD5 to generate a sequence
     * of hex characters so that we can interpret
     * the digested @p name as an MinecraftUUID.
     * It is then a deterministic function.
     * See more at crypto::md5Digest
     * @param name the name to generate from
     * @return MinecraftUUID the newly created MinecraftUUID
     */
    static MinecraftUUID fromUsername(const std::string &name);
    /**
     * @brief Contructs an MinecraftUUID from an hex string
     *
     * Interprets the hex string as an MinecraftUUID, whether
     * it contains hyphens or not.
     * @param hexString the string to construct from
     * @return MinecraftUUID the newly created MinecraftUUID
     */
    static MinecraftUUID fromHex(const std::string &hexString);
    /**
     * @brief Constructs an MinecraftUUID from the raw bytes
     *
     * @param buff a pointer to the MinecraftUUID
     * @return MinecraftUUID the newly created MinecraftUUID
     */
    static MinecraftUUID fromBytes(const std::byte *buff);

    /**
     * @brief Loads MinecraftUUID to the lua state
     *
     * @param state the lua state to load to
     * @param namespaceName the namespace to load to
     */
    static void loadLua(lua_State *state, const char *namespaceName);
};

#endif // MINESERVER_UUID_H