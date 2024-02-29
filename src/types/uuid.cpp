/**
 * @file uuid.cpp
 * @author Lygaen
 * @brief The file containing MinecraftUUID logic
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "uuid.h"
#include <utils/crypto.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

MinecraftUUID::MinecraftUUID() : bytes({})
{
    MinecraftUUID temp = MinecraftUUID::newRandom();
    std::copy(temp.bytes.begin(), temp.bytes.end(), bytes.begin());
}

bool MinecraftUUID::operator==(MinecraftUUID const &other)
{
    return std::equal(other.bytes.begin(), other.bytes.end(), bytes.begin());
}

std::string MinecraftUUID::getFull() const
{
    std::stringstream stream;
    stream << std::hex << std::setfill('0');
    stream << std::setw(2) << int(bytes[0]);
    stream << std::setw(2) << int(bytes[1]);
    stream << std::setw(2) << int(bytes[2]);
    stream << std::setw(2) << int(bytes[3]);
    stream << '-';

    stream << std::setw(2) << int(bytes[4]);
    stream << std::setw(2) << int(bytes[5]);
    stream << '-';

    stream << std::setw(2) << int(bytes[6]);
    stream << std::setw(2) << int(bytes[7]);
    stream << '-';

    stream << std::setw(2) << int(bytes[8]);
    stream << std::setw(2) << int(bytes[9]);
    stream << '-';

    stream << std::setw(2) << int(bytes[10]);
    stream << std::setw(2) << int(bytes[11]);
    stream << std::setw(2) << int(bytes[12]);
    stream << std::setw(2) << int(bytes[13]);
    stream << std::setw(2) << int(bytes[14]);
    stream << std::setw(2) << int(bytes[15]);

    return stream.str();
}

std::string MinecraftUUID::getTrimmed() const
{
    std::string s = getFull();
    s.erase(std::remove(s.begin(), s.end(), '-'), s.end());
    return s;
}

const std::byte *MinecraftUUID::getBytes() const
{
    return bytes.data();
}

MinecraftUUID MinecraftUUID::newRandom()
{
    std::unique_ptr<std::byte[]> bytes = crypto::randomSecure(16);
    return MinecraftUUID::fromBytes(bytes.get());
}

MinecraftUUID MinecraftUUID::fromUsername(const std::string &name)
{
    std::string hex = crypto::md5Digest(name);

    return MinecraftUUID::fromHex(hex);
}

/**
 * @brief Convert one character to byte value in Base64 format
 *
 * @param ch the character
 * @return char the value
 */
char hexDigitToChar(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';

    if (ch >= 'a' && ch <= 'f')
        return ch - 87;

    if (ch >= 'A' && ch <= 'F')
        return ch - 55;

    return 0;
}

/**
 * @brief Check whether the character is a valid Base64 character
 *
 * @param ch the character to check
 * @return true the character is valid
 * @return false the character is not valid
 */
bool isValidHexChar(char ch)
{
    return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
}

/**
 * @brief Converts two chars to one byte
 *
 * @param a the first char
 * @param b the second char
 * @return unsigned char the byte value
 */
unsigned char hexPairToChar(char a, char b)
{
    return hexDigitToChar(a) * 16 + hexDigitToChar(b);
}

MinecraftUUID MinecraftUUID::fromHex(const std::string &uuidString)
{
    std::array<std::byte, 16> bytes{};
    bool firstChar = true;
    int length = 0;
    char lastChar;

    for (auto &c : uuidString)
    {
        if (c == '-')
            continue;

        if (!isValidHexChar(c) ||
            length >= 16)
            return MinecraftUUID(std::array<std::byte, 16>({}));

        if (firstChar)
        {
            lastChar = c;
            firstChar = false;
        }
        else
        {
            unsigned char b = hexPairToChar(lastChar, c);
            bytes[length] = *reinterpret_cast<std::byte *>(&b);
            length++;
            firstChar = true;
        }
    }

    if (length < 16)
        return MinecraftUUID(std::array<std::byte, 16>({}));
    return MinecraftUUID(bytes);
}

MinecraftUUID MinecraftUUID::fromBytes(const std::byte *buff)
{
    MinecraftUUID out(std::array<std::byte, 16>({}));
    std::copy(const_cast<std::byte *>(buff), const_cast<std::byte *>(buff) + 16, out.bytes.data());
    return out;
}

void MinecraftUUID::loadLua(lua_State *state, const char *namespaceName)
{
    luabridge::getGlobalNamespace(state)
        .beginNamespace(namespaceName)
        .beginClass<MinecraftUUID>("MinecraftUUID")
        .addConstructor([](void *ptr)
                        { return new (ptr) MinecraftUUID(MinecraftUUID::newRandom()); })
        .addFunction("getFull", &MinecraftUUID::getFull)
        .addFunction("getTrimmed", &MinecraftUUID::getTrimmed)
        .addFunction("getBytes", &MinecraftUUID::getBytes)
        .endClass()
        .endNamespace();
}
