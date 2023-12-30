/**
 * @file angle.hpp
 * @author Mathieu Cayeux
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
};

#endif // MINESERVER_ANGLE_H