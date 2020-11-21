//
// Created by Tal on 19/11/2020.
//

#include "Point.h"

Point::Point(const Coordinates &coordinates) : Geometry(), coordinates(coordinates) {}

const Coordinates &Point::getCoordinates() const {
    return coordinates;
}
