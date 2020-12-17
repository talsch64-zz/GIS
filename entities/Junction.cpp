#include "Junction.h"
#include "JsonHandlers/Serializers/EntityJsonSerializer.h"

Junction::Junction(const EntityId &id, const std::string &name, const std::string &description,
                   const std::vector<std::string> &categoryTags, std::unique_ptr<Point> geometry) :
        Entity(id, name, description, categoryTags, "Junction"), geometry(std::move(geometry)) {}

rapidjson::Value Junction::toJson(rapidjson::Document::AllocatorType &allocator) {
    return entityJsonSerializer->toJson(this, allocator);
}

const std::unique_ptr<Geometry> &Junction::getGeometry() const {
    return (const std::unique_ptr<Geometry> &) geometry;
}
