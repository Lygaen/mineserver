/**
 * @file angle.hpp
 * @author Lygaen
 * @brief The file for angle logic
 * @version 0.1
 * @date 2023-12-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_ANGLE_H
#define MINESERVER_ANGLE_H

#include <cstddef>
#include <plugins/luaheaders.h>

/**
 * @brief Angle class holder
 *
 * Minecraft-way to hold angles
 * as a single byte.
 */
class Angle
{
private:
    std::byte value;

public:
    /**
     * @brief Construct a new Angle object
     *
     * Angle is 0.
     */
    Angle() : value((std::byte)0) {}
    /**
     * @brief Construct a new Angle object from degrees
     *
     * @param degrees the degrees of the angle
     */
    Angle(float degrees) : value((std::byte)(degrees * 256.f / 360.f)) {}
    /**
     * @brief Construct a new Angle object from byte
     *
     * @param value the byte value of the angle
     */
    Angle(std::byte value) : value(value) {}
    /**
     * @brief Destroy the Angle object
     *
     */
    ~Angle() = default;

    /**
     * @brief Get the Degrees of the Angle
     *
     * @return float the degrees of the angle
     */
    float getDegrees() const
    {
        return ((float)value) * 360.f / 256.f;
    }

    /**
     * @brief Get the Byte representation
     *
     * @return std::byte the angle as a byte
     */
    std::byte getByte() const
    {
        return value;
    }

    /**
     * @brief Loads Angle to lua state
     *
     * @param state lua state
     * @param namespaceName base namespace name
     */
    static void loadLua(lua_State *state, const char *namespaceName)
    {
        luabridge::getGlobalNamespace(state)
            .beginNamespace(namespaceName)
            .beginClass<Angle>("Angle")
            .addConstructor<void(float), void(std::byte)>()
            .addFunction("getDegrees", &Angle::getDegrees)
            .addFunction("getByte", &Angle::getByte)
            .endClass()
            .endNamespace();
    }
};

#endif // MINESERVER_ANGLE_H