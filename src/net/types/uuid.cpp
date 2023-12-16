#include "uuid.h"
#include <utils/crypto.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

std::string UUID::getFull() const
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

std::string UUID::getTrimmed() const
{
    std::string s = getFull();
    s.erase(std::remove(s.begin(), s.end(), '-'), s.end());
    return s;
}

const std::byte *UUID::getBytes() const
{
    return bytes.data();
}

UUID UUID::newRandom()
{
    std::unique_ptr<std::byte[]> bytes = crypto::randomSecure(16);
    return UUID::fromBytes(bytes.get());
}

UUID UUID::fromUsername(const std::string &name)
{
    std::string hex = crypto::md5Digest(name);

    return UUID::fromHex(hex);
}

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

bool isValidHexChar(char ch)
{
    return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
}

// converts the two hexadecimal characters to an unsigned char (a byte)
unsigned char hexPairToChar(char a, char b)
{
    return hexDigitToChar(a) * 16 + hexDigitToChar(b);
}

UUID UUID::fromHex(const std::string &uuidString)
{
    std::array<std::byte, 16> bytes;
    bool firstChar = true;
    int length = 0;
    char lastChar;

    for (auto &c : uuidString)
    {
        if (c == '-')
            continue;

        if (!isValidHexChar(c) ||
            length >= 16)
            return UUID({});

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
        return UUID({});
    return UUID(bytes);
}

UUID UUID::fromBytes(const std::byte *buff)
{
    UUID out({});
    std::copy(const_cast<std::byte *>(buff), const_cast<std::byte *>(buff) + 16, out.bytes.data());
    return out;
}

void UUID::loadLua(lua_State *state, const char *namespaceName)
{
    luabridge::getGlobalNamespace(state)
        .beginNamespace(namespaceName)
        .beginClass<UUID>("UUID")
        .addConstructor([](void *ptr)
                        { return new (ptr) UUID(UUID::newRandom()); })
        .addFunction("getFull", &UUID::getFull)
        .addFunction("getTrimmed", &UUID::getTrimmed)
        .addFunction("getBytes", &UUID::getBytes)
        .endClass()
        .endNamespace();
}
