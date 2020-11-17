#include "Geometry.h"
#include "../JsonHandlers/Serializers/GeometryJsonSerializer.h"

Geometry::Geometry(std::vector<Coordinates> coordinates) : coordinates(coordinates),
                                                           geometryJsonSerializer(new GeometryJsonSerializer()) {}

std::vector<Coordinates> Geometry::getCoordinates() {
    return coordinates;
}

Geometry::~Geometry() {}