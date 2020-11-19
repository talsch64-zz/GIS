#include "Junction.h"
#include "JsonHandlers/Serializers/EntityJsonSerializer.h"

Junction::Junction(const std::string &id, const std::string &name, const std::string &description,
                       const std::vector<std::string> &categoryTags, std::unique_ptr<Geometry> geometry):
                       Entity(id, name, description, categoryTags, std::move(geometry), "Junction"){}

rapidjson::Value Junction::toJson(rapidjson::Document::AllocatorType &allocator) {
    return entityJsonSerializer->toJson(this, allocator);
}


