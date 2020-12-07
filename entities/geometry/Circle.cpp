#include "Circle.h"
#include "../../CoordinatesMath.h"
#include "../JsonHandlers/Serializers/GeometryJsonSerializer.h"
#include "../search/Grid.h"
#include "../../search/TopologicalSearch.h"


Circle::Circle(const Coordinates &coordinates, const Meters &radius) : Geometry(), radius(radius),
                                                                      coordinates(coordinates) {}
rapidjson::Value Circle::toJson(rapidjson::Document::AllocatorType &allocator) {
    return geometryJsonSerializer->toJson(this, allocator);
}

Coordinates Circle::getCenter() const {
    return coordinates;
}

std::vector<Grid::GridCell> Circle::getGridCells(const Grid *grid) {
    return grid->getGeometryGridCells(*this);
}

bool Circle::isInCircle(const TopologicalSearch *topologicalSearch, const Coordinates &searchCenter, Meters searchRadius) const {
    return topologicalSearch->isInCircle(searchCenter, searchRadius, *this);
}

Coordinates Circle::getClosestPoint(const Coordinates &coordinates) const {
    return CoordinatesMath::closestPointOnCircle(coordinates, getCenter(), getRadius());
}

