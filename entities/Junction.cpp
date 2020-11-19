#include "Junction.h"
#include "JsonHandlers/Serializers/EntityJsonSerializer.h"

Junction::Junction(const std::string &id, const std::string &name, const std::string &description,
                   const std::vector<std::string> &categoryTags, Coordinates coordinates) : Entity(
        id, name, description, categoryTags), coordinates(coordinates) {}

rapidjson::Value Junction::toJson(rapidjson::Document::AllocatorType &allocator) {
    return entityJsonSerializer->toJson(this, allocator);
}

const Coordinates &Junction::getCoordinates() const {
    return coordinates;
}
