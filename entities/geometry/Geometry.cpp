#include "Geometry.h"
#include "../JsonHandlers/Serializers/GeometryJsonSerializer.h"

Geometry::Geometry(std::vector<Coordinates> coordinates) : coordinates(coordinates),
                                                           geometryJsonSerializer(new GeometryJsonSerializer()) {}

Geometry::Geometry(const Coordinates &coordinates) : geometryJsonSerializer(new GeometryJsonSerializer()) {
    this->coordinates.push_back(coordinates);
}


std::vector<Coordinates> Geometry::getCoordinates() {
    return coordinates;
}

Geometry::~Geometry() {}