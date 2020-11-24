//
// Created by Tal on 19/11/2020.
//

#include "Point.h"
#include "../JsonHandlers/Serializers/GeometryJsonSerializer.h"

Point::Point(const Coordinates &coordinates) : Geometry(), coordinates(coordinates) {}

const Coordinates &Point::getCoordinates() const {
    return coordinates;
}

rapidjson::Value Point::toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) {
    return geometryJsonSerializer->toJson(this, allocator);
}
