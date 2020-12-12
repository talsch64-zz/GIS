#include "Junction.h"
#include "JsonHandlers/Serializers/EntityJsonSerializer.h"

Junction::Junction(const EntityId &id, const std::string &name, const std::string &description,
                       const std::vector<std::string> &categoryTags, std::unique_ptr<Geometry> geometry):
                       Entity(id, name, description, categoryTags, std::move(geometry), "Junction"), ways(std::vector<EntityId>()){}

rapidjson::Value Junction::toJson(rapidjson::Document::AllocatorType &allocator) {
    return entityJsonSerializer->toJson(this, allocator);
}

void Junction::addWay(const EntityId &wayId) {
    ways.push_back(wayId);
}

const std::vector<EntityId> &Junction::getWays() const {
    return ways;
}


