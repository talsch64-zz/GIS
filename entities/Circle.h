//
// Created by Tal on 05/11/2020.
//

#ifndef EX1_CIRCLE_H
#define EX1_CIRCLE_H

#include "geometry/Geometry.h"

class Circle: public Geometry {
    Meters radius;
public:
    Circle(const std::vector<Coordinates> &coordinates, const Meters radius) : Geometry(coordinates), radius(radius) {}

    const Meters &getRadius() const {
        return radius;
    }
};
#endif //EX1_CIRCLE_H
