
#ifndef EX1_WAY_H
#define EX1_WAY_H

#include "Entity.h"
#include "../../Common/GISNamedTypes.h"
#include "geometry/PointList.h"
#include "../../Common/Restrictions.h"
#include "../../Common/AbstractWay.h"

/// Entity derived class which represents a Way in the GIS application
/// Way's geometry is a Circle that is represented by a radius and center coordinates

enum class TrafficDirection {
    unidirectional, bidirectional
};

class Way : public Entity, public AbstractWay {
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
    mutable std::optional<std::vector<Meters>> lengthSegments;

public:

    Way(const EntityId &id, const std::string &name, const std::string &description,
        const std::vector<std::string> &categoryTags, std::unique_ptr<PointList> geometry, EntityId from,
        EntityId to, TrafficDirection direction, int speedLimit, bool tollRoad, bool highway, std::vector<std::string> restricted);

    const std::optional<std::vector<Meters>> &getLengthSegments() const;

    TrafficDirection getDirection() const;

    const Coordinates &getFromJunctionCoordinates() const override;

    const Coordinates &getToJunctionCoordinates() const override;


    /**
     * @brief
     *
     * @return true - Way is Bidirectional
     * @return false - Way is Unidirectional
     */
    bool isBidirectional() const override;

    /**
     * @brief Get the Speed Limit set on Way
     *
     * @return int
     */
    int getSpeedLimit() const override;

    /**
     * @brief
     *
     * @return true - Way is an Highway
     * @return false - Way is not an Highway
     */
    bool isHighway() const override;

    bool isToll() const override;

    const std::vector<std::string> &getRestricted() const;

    rapidjson::Value toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) override;

    /**
     * Get from, to junctions as a pair
     * @return from, to pair
     */
    std::pair<EntityId, EntityId> getJunctions() const override;

    /**
     * Get the length of the way in meters
     * @return length in meters
     */
    Meters getLength() const override;

    const std::unique_ptr<Geometry> &getGeometry() const override;

    const EntityId &getId() const override;

private:
    std::pair<Meters, Meters> getSegmentPartsOnWay(std::size_t segment, const Coordinates &c) const override;
};

#endif //EX1_WAY_H
