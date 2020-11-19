
#include "JsonHandlers/Serializers/EntityJsonSerializer.h"

#include "Way.h"

Way::Way(const std::string &id, const std::string &name, const std::string &description,
         const std::vector<std::string> &categoryTags, std::unique_ptr<Geometry> geometry, const std::string &from, const std::string &to,
         const std::string &direction, int speedLimit, bool tollRoad,
         const std::vector<std::string> &restricted) : Entity(id, name, description, categoryTags, std::move(geometry), "Way"),
                                                       from(from), to(to), direction(direction), speedLimit(speedLimit),
                                                       tollRoad(tollRoad), restricted(restricted) {}


rapidjson::Value Way::toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) {
    return entityJsonSerializer->toJson(this, allocator);
}

const EntityId &Way::getFrom() const {
    return from;
}

const EntityId &Way::getTo() const {
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
