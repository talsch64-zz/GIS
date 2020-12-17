//
// Created by Tal on 19/11/2020.
//

#include "Point.h"
#include "../JsonHandlers/Serializers/GeometryJsonSerializer.h"
#include "../search/Grid.h"

Point::Point(const Coordinates &coordinates) : Geometry(), coordinates(coordinates) {}

const Coordinates &Point::getCoordinates() const {
    return coordinates;
}

rapidjson::Value Point::toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    return geometryJsonSerializer->toJson(this, allocator);
}

std::vector<Grid::GridCell> Point::getGridCells(const Grid *grid) const {
    return grid->getGeometryGridCells(*this);
}

Coordinates Point::getClosestPoint(const Coordinates &coord) const {
    coord.latitude();
    return coordinates;
}

bool Point::isInCircle(const TopologicalSearch *topologicalSearch, const Coordinates &center, Meters radius) const {
    return topologicalSearch->isInCircle(center, radius, *this);
}
