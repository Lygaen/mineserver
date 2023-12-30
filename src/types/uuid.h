/**
 * @file uuid.h
 * @author Lygaen
 * @brief The file containing UUID logic
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
 * @brief UUID compliant data holder
 *
 * Data holder for an [RFC 4122](https://www.rfc-editor.org/rfc/pdfrfc/rfc4122.txt.pdf)
 * compliant UUID.
 */
class UUID
{
private:
    std::array<std::byte, 16> bytes;
    UUID(const std::array<std::byte, 16> &bytes) : bytes(bytes) {}

public:
    /**
     * @brief Construct a new UUID object
     *
     * In reality, just a wrapper around ::newRandom()
     */
    UUID();

    /**
     * @brief Equality operator
     *
     * @param other the other member
     * @return true the members are equal
     * @return false the member are not equal
     */
    bool operator==(UUID const &other);

    /**
     * @brief Get the hex of the UUID with hyphens
     *
     * It will resemble something such as :
     * @code
     * 5999de96-5ade-4a40-bac9-29f690620fcc
     * @endcode
     * @return std::string the hex with hyphens
     */
    std::string getFull() const;
    /**
     * @brief Get the hex of the UUID without hyphens
     *
     * It will resemble something such as :
     * @code
     * 5999de965ade4a40bac929f690620fcc
     * @endcode
     * @return std::string the hex without hyphens
     */
    std::string getTrimmed() const;
    /**
     * @brief Get the raw bytes of the UUID
     *
     * @return const std::byte* pointer to byte data
     */
    const std::byte *getBytes() const;

    /**
     * @brief Constructs a new random UUID
     *
     * @return UUID the newly created UUID
     */
    static UUID newRandom();
    /**
     * @brief Constructs an UUID from an username
     *
     * In reality, uses MD5 to generate a sequence
     * of hex characters so that we can interpret
     * the digested @p name as an UUID.
     * It is then a deterministic function.
     * See more at crypto::md5Digest
     * @param name the name to generate from
     * @return UUID the newly created UUID
     */
    static UUID fromUsername(const std::string &name);
    /**
     * @brief Contructs an UUID from an hex string
     *
     * Interprets the hex string as an UUID, whether
     * it contains hyphens or not.
     * @param hexString the string to construct from
     * @return UUID the newly created UUID
     */
    static UUID fromHex(const std::string &hexString);
    /**
     * @brief Constructs an UUID from the raw bytes
     *
     * @param buff a pointer to the UUID
     * @return UUID the newly created UUID
     */
    static UUID fromBytes(const std::byte *buff);

    /**
     * @brief Loads UUID to the lua state
     *
     * @param state the lua state to load to
     * @param namespaceName the namespace to load to
     */
    static void loadLua(lua_State *state, const char *namespaceName);
};

#endif // MINESERVER_UUID_H