#include "Geometry.h"

Geometry::Geometry(const std::vector<Coordinates> &coordinates) : coordinates(
        new std::vector<Coordinates>(coordinates)) {}

const std::vector<Coordinates> *Geometry::getCoordinates() const {
    return coordinates;
}
