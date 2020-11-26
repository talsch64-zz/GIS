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
    Coordinates center = getCenter();
    Meters radius = getRadius();
    Meters distance = CoordinatesMath::calculateDistance(center, coordinates);
    if (distance <= radius) {
        return coordinates;
    }
    double bearing = CoordinatesMath::calculateBearing(center, coordinates);
    return CoordinatesMath::calculateCoordinatesByDistanceAndBearing(center, radius, bearing);
}

