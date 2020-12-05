//
// Created by royar on 10/11/2020.
//

#ifndef EX1_POLYGON_H
#define EX1_POLYGON_H

#include "Geometry.h"

class Polygon: public Geometry {
public:
    Polygon(const std::vector<Coordinates> &coordinates);
};


#endif //EX1_POLYGON_H
