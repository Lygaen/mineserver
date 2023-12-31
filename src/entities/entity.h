/**
 * @file entity.h
 * @author Lygaen
 * @brief The file having entities logic
 * @version 0.1
 * @date 2023-12-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_ENTITY_H
#define MINESERVER_ENTITY_H

#include <types/vector.hpp>
#include <types/angle.hpp>
#include <types/uuid.h>

/**
 * @brief Interface Entity
 *
 * Interface for all entities, implements
 * basic things that all entities should have
 */
class IEntity
{
public:
    /**
     * @brief Destroy the IEntity object
     *
     */
    virtual ~IEntity() = default;

    /**
     * @brief Position of the entity
     *
     */
    Vecf position;
    /**
     * @brief Yaw angle of the entity
     *
     * How the entity is oriented in space,
     * horizontally.
     */
    Angle yaw;
    /**
     * @brief Pitch angle of the entity
     *
     * How the entity is oriented in space,
     * vertically.
     */
    Angle pitch;
    /**
     * @brief UUID of the entity
     *
     * Unique identifier for the entity
     */
    UUID uuid;

    // TODO implement everything in Lua
};

/**
 * @brief Interface Living Entity
 *
 * Interface for all living entity, implements
 * basic things that all living entities should
 * have
 */
class ILiving : public IEntity
{
public:
    /**
     * @brief Destroy the ILiving object
     *
     */
    virtual ~ILiving() = default;
    // TODO implement everything in Lua
};

#endif // MINESERVER_ENTITY_H