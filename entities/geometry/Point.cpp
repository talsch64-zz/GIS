//
// Created by Tal on 19/11/2020.
//

#include "Point.h"

Point::Point(const Coordinates &coordinates) : coordinates(coordinates) {}

const Coordinates &Point::getCoordinates() const {
    return coordinates;
}

rapidjson::Value Point::toJson(rapidjson::Document::AllocatorType &allocator) {
    return rapidjson::Value();
}
