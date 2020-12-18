
#include "JsonHandlers/Serializers/EntityJsonSerializer.h"

#include "Way.h"

#include <utility>

Way::Way(const EntityId &id, const std::string &name, const std::string &description,
         const std::vector<std::string> &categoryTags, std::unique_ptr<PointList> geometry, EntityId from, EntityId to,
         std::string direction, int speedLimit, bool tollRoad,
         std::vector<std::string> restricted) : Entity(id, name, description, categoryTags, "Way"),
                                                from(std::move(from)), to(std::move(to)), geometry(std::move(geometry)),
                                                direction(std::move(direction)), speedLimit(speedLimit),
                                                tollRoad(tollRoad), restricted(std::move(restricted)) {}

const std::unique_ptr<Geometry> &Way::getGeometry() const {
    return (const std::unique_ptr<Geometry> &) geometry;
}

rapidjson::Value Way::toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) {
    return entityJsonSerializer->toJson(this, allocator);
}

const EntityId &Way::getFromJunctionId() const {
    return from;
}

const EntityId &Way::getToJunctionId() const {
    return to;
}

const std::string &Way::getDirection() const {
    return direction;
}

int Way::getSpeedLimit() const {
    return speedLimit;
}

bool Way::isTollRoad() const {
    return tollRoad;
}

const std::vector<std::string> &Way::getRestricted() const {
    return restricted;
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

bool Way::isRestricted(const Restrictions &restrictions) const {
    for (std::string restriction : restrictions.getRestrictions()) {
        if (std::find(restricted.begin(), restricted.end(), restriction) != restricted.end()) {
            return true;
        }
    }
    return false;
}

bool Way::isBidirectional() const {
    return direction == "bidirectional";
}

bool Way::isHighway() const {
    auto categoryTags = getCategoryTags();
    bool highway = std::find(categoryTags.begin(), categoryTags.end(), "highway") != categoryTags.end();
    return highway;
}

const Coordinates Way::getFromJunctionCoordinates() const {
    return geometry->getPoints().front();
}

const Coordinates Way::getToJunctionCoordinates() const {
    return geometry->getPoints().back();
}

Minutes Way::getTime() const {
    return Minutes((double) getLength() / kmh_to_mpm(getSpeedLimit()));

}

double Way::kmh_to_mpm(int speed) {
    return (double) speed * 1000 / 60;
}
