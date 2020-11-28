#include "Circle.h"
#include "CoordinatesMath.h"
#include "../JsonHandlers/Serializers/GeometryJsonSerializer.h"
#include "../search/Grid.h"


Circle::Circle(const Coordinates &coordinates, const Meters &radius) : Geometry(), radius(radius),
                                                                      coordinates(coordinates) {}
rapidjson::Value Circle::toJson(rapidjson::Document::AllocatorType &allocator) {
    return geometryJsonSerializer->toJson(this, allocator);
}

Coordinates Circle::getCenter() {
    return coordinates;
}

std::vector<Grid::GridCell> Circle::getGridCells(const Grid *grid) {
    return grid->getGeometryGridCells(*this);
}

Coordinates Circle::getClosestPoint(const Coordinates &coordinates) {
    return CoordinatesMath::closestPointOnCircle(coordinates, getCenter(), getRadius());
}

