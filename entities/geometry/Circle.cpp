#include "Circle.h"
#include "../JsonHandlers/Serializers/GeometryJsonSerializer.h"

rapidjson::Value Circle::toJson(rapidjson::Document::AllocatorType &allocator) {
    return geometryJsonSerializer->toJson(this, allocator);
}

Coordinates Circle::getCenter() {
    return coordinates[0];
}