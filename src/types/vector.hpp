/**
 * @file vector.hpp
 * @author Lygaen
 * @brief The file handling vector logic
 * @version 0.1
 * @date 2023-12-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_VECTOR_HPP
#define MINESERVER_VECTOR_HPP

#include <cstdint>
#include <type_traits>
#include <plugins/luaheaders.h>

/**
 * @brief Vector 3 (x, y, z) with variable type
 *
 * @tparam T the type to use for x, y, z coordinates
 */
template <typename T>
class Vec
{
public:
    static_assert(std::is_arithmetic_v<T>, "Type should be a number");
    /**
     * @brief X coordinate
     *
     */
    T x;
    /**
     * @brief Y coordinate
     *
     */
    T y;
    /**
     * @brief Z coordinate
     *
     */
    T z;

    /**
     * @brief Construct a new Vec object
     *
     * All coordinates are set as 0.
     */
    Vec() : x(0), y(0), z(0) {}
    /**
     * @brief Construct a new Vec object
     *
     * The z coordinate is set to 0.
     * @param x the x coordinate
     * @param y the y coordinate
     */
    Vec(T x, T y) : x(x), y(y), z(0) {}
    /**
     * @brief Construct a new Vec object
     *
     * @param x the x coordinate
     * @param y the y coordinate
     * @param z the z coordinate
     */
    Vec(T x, T y, T z) : x(x), y(y), z(z) {}
    /**
     * @brief Destroy the Vec object
     *
     */
    ~Vec() = default;

    /**
     * @brief Construct a new Vec object from another
     *
     * @tparam O the type of the other vector
     * @param other the other vector
     */
    template <typename O>
    Vec(const Vec<O> &other)
    {
        x = (T)other.x;
        y = (T)other.y;
        z = (T)other.z;
    }

    /**
     * @brief Vector Addition operator
     *
     * @tparam O the type of the other vector
     * @param other the other vector
     * @return Vec& the returned vector
     */
    template <typename O>
    Vec &operator+(const Vec<O> &other)
    {
        x = (T)(x + other.x);
        y = (T)(y + other.y);
        z = (T)(z + other.z);
        return *this;
    }

    /**
     * @brief Vector Substraction operator
     *
     * @tparam O the type of the other vector
     * @param other the other vector
     * @return Vec& the returned vector
     */
    template <typename O>
    Vec &operator-(const Vec<O> &other)
    {
        x = (T)(x - other.x);
        y = (T)(y - other.y);
        z = (T)(z - other.z);
        return *this;
    }

    /**
     * @brief Vector Multiplication operator
     *
     * @tparam O the type of the other vector
     * @param other the other vector
     * @return Vec& the returned vector
     */
    template <typename O>
    Vec &operator*(const Vec<O> &other)
    {
        x = (T)(x * other.x);
        y = (T)(y * other.y);
        z = (T)(z * other.z);
        return *this;
    }

    /**
     * @brief Vector Division operator
     *
     * @tparam O the type of the other vector
     * @param other the other vector
     * @return Vec& the returned vector
     */
    template <typename O>
    Vec &operator/(const Vec<O> &other)
    {
        x = (T)(x / other.x);
        y = (T)(y / other.y);
        z = (T)(z / other.z);
        return *this;
    }

    /**
     * @brief Scalar Multiplication operator
     *
     * @tparam O the type of the scalar (should be arithmetic type)
     * @param other the other scalar
     * @return Vec& the returned vector
     */
    template <typename O>
    Vec &operator*(const O &other)
    {
        static_assert(std::is_arithmetic_v<O>, "Type should be a number");
        x *= other;
        y *= other;
        z *= other;
        return *this;
    }

    /**
     * @brief Scalar Division operator
     *
     * @tparam O the type of the scalar (should be arithmetic type)
     * @param other the other scalar
     * @return Vec& the returned vector
     */
    template <typename O>
    Vec &operator/(const O &other)
    {
        static_assert(std::is_arithmetic_v<O>, "Type should be a number");
        x /= other;
        y /= other;
        z /= other;
        return *this;
    }
};

/**
 * @brief Vector 3 Float
 *
 * All coordinates are represented as floats.
 */
typedef Vec<float> Vecf;
/**
 * @brief Vector 3 Int
 *
 * All coordinates are represented as a 32-byte integer.
 */
typedef Vec<std::int32_t> Veci32;

/**
 * @brief Loads Vector to lua state
 *
 * @param state lua state
 * @param namespaceName base namespace name
 */
static void loadVectorLua(lua_State *state, const char *namespaceName)
{
    luabridge::getGlobalNamespace(state)
        .beginNamespace(namespaceName)
        .beginClass<Vecf>("Vecf")
        .addConstructor<void(), void(float, float), void(float, float, float)>()
        .addProperty("x", &Vecf::x)
        .addProperty("y", &Vecf::y)
        .addProperty("z", &Vecf::z)
        .endClass()
        .beginClass<Veci32>("Veci32")
        .addConstructor<void(), void(std::int32_t, std::int32_t), void(std::int32_t, std::int32_t, std::int32_t)>()
        .addProperty("x", &Veci32::x)
        .addProperty("y", &Veci32::y)
        .addProperty("z", &Veci32::z)
        .endClass()
        .endNamespace();
}

#endif // MINESERVER_VECTOR_HPP