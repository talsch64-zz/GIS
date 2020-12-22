
#ifndef EX1_WAY_H
#define EX1_WAY_H

#include "Entity.h"
#include "../GISNamedTypes.h"
#include "geometry/PointList.h"
#include "../Restrictions.h"

/// Entity derived class which represents a Way in the GIS application
/// Way's geometry is a Circle that is represented by a radius and center coordinates

enum class TrafficDirection {
    unidirectional, bidirectional
};

class Way : public Entity {
protected:
    EntityId from;
    EntityId to;
    std::unique_ptr<PointList> geometry;
    TrafficDirection direction;
    int speedLimit;
    bool tollRoad;
    bool highway;
    std::vector<std::string> restricted;
    mutable std::optional<Meters> length;

public:

    Way(const EntityId &id, const std::string &name, const std::string &description,
        const std::vector<std::string> &categoryTags, std::unique_ptr<PointList> geometry, EntityId from,
        EntityId to, TrafficDirection direction, int speedLimit, bool tollRoad, bool highway, std::vector<std::string> restricted);

    const EntityId &getFromJunctionId() const;

    const EntityId &getToJunctionId() const;

    const TrafficDirection getDirection() const;

    const Coordinates getFromJunctionCoordinates() const;

    const Coordinates getToJunctionCoordinates() const;


    /**
     * @brief
     *
     * @return true - Way is Bidirectional
     * @return false - Way is Unidirectional
     */
    bool isBidirectional() const;

    /**
     * @brief Get the Speed Limit set on Way
     *
     * @return int
     */
    int getSpeedLimit() const;

    /**
     * @brief
     *
     * @return true - Way is an Highway
     * @return false - Way is not an Highway
     */
    bool isHighway() const;

    bool isTollRoad() const;

    const std::vector<std::string> &getRestricted() const;

    rapidjson::Value toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) override;

    /**
     * Get from, to junctions as a pair
     * @return from, to pair
     */
    std::pair<EntityId, EntityId> getJunctions() const;

    /**
     * Get the length of the way in meters
     * @return length in meters
     */
    Meters getLength() const;

    Minutes getTime() const;

    const std::unique_ptr<Geometry> &getGeometry() const override;

    /**
     * Does the way contain any of the specified restrictions
     * @param restrictions restrictions Restrictions object to check against
     * @return is the way restricted
     */
    bool isRestricted(const Restrictions &restrictions) const;

    /**
     * @brief converts speed kilometers per hour to meters per minute
     * @param speed
     * @return Minutes
     */

};

#endif //EX1_WAY_H
