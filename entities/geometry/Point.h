//
// Created by Tal on 19/11/2020.
//

#ifndef EX1_POINT_H
#define EX1_POINT_H
#include "Geometry.h"
#include "../../GISdefs.h"""


class Point: public Geometry {
    Coordinates coordinates;
public:
    Point(const Coordinates &coordinates, const Coordinates &coordinates1);
};
#endif //EX1_POINT_H
