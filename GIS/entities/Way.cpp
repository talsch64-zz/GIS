
#include "JsonHandlers/Serializers/EntityJsonSerializer.h"

#include "Way.h"
#include "../../UserCommon/Utils.h"
#include "../../Common/CoordinatesMath.h"

#include <utility>

Way::Way(const EntityId &id, const std::string &name, const std::string &description,
         const std::vector<std::string> &categoryTags, std::unique_ptr<PointList> geometry, EntityId from, EntityId to,
         TrafficDirection direction, int speedLimit, bool tollRoad, bool highway,
         std::vector<std::string> restricted) : Entity(id, name, description, categoryTags, "Way"),
                                                from(std::move(from)), to(std::move(to)), geometry(std::move(geometry)),
                                                direction(direction), speedLimit(speedLimit),
                                                tollRoad(tollRoad), highway(highway),
                                                restricted(std::move(restricted)) {}

const EntityId &Way::getId() const {
    return Entity::getId();
}

const std::unique_ptr<Geometry> &Way::getGeometry() const {
    return (const std::unique_ptr<Geometry> &) geometry;
}

rapidjson::Value Way::toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) {
    return entityJsonSerializer->toJson(this, allocator);
}

TrafficDirection Way::getDirection() const {
    return direction;
}

int Way::getSpeedLimit() const {
    return speedLimit;
}

bool Way::isToll() const {
    return tollRoad;
}

const std::vector<std::string> &Way::getRestricted() const {
    return restricted;
}

std::pair<Meters, Meters> Way::getSegmentPartsOnWay(std::size_t segment, const Coordinates &c) const {
    Meters distanceFromStart = geometry->getDistanceFromStart(segment, c);
    Meters distanceFromEnd = geometry->getDistanceFromEnd(segment, c);
    return std::make_pair(distanceFromStart, distanceFromEnd);
}

std::pair<EntityId, EntityId> Way::getJunctions() const {
    return std::pair<EntityId, EntityId>(from, to);
}

Meters Way::getLength() const {
    if (!length.has_value()) {
        length = geometry->getLength();
    }
    return length.value();
}

bool Way::isBidirectional() const {
    return direction == TrafficDirection::bidirectional;
}

bool Way::isHighway() const {
    return highway;
}

const Coordinates &Way::getFromJunctionCoordinates() const {
    return geometry->getPoints().front();
}

const Coordinates &Way::getToJunctionCoordinates() const {
    return geometry->getPoints().back();
}

size_t Way::getContainingSegment(Coordinates coordinates) {
    return geometry->getContainingSegment(coordinates);
}


