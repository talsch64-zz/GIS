#include "Circle.h"
#include "../JsonHandlers/Serializers/GeometryJsonSerializer.h"



Circle::Circle(const Coordinates &coordinates, const Meters radius) : Geometry("Circle"), radius(radius), coordinates(coordinates) {}

rapidjson::Value Circle::toJson(rapidjson::Document::AllocatorType &allocator) {
    return geometryJsonSerializer->toJson(this, allocator);
}

Coordinates Circle::getCenter() {
    return coordinates;
}