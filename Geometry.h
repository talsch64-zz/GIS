//
// Created by Tal on 05/11/2020.
//

#ifndef EX1_GEOMETRY_H
#define EX1_GEOMETRY_H


#include <vector>
#include "GIS.h"

class Geometry {
    const std::vector<Coordinates> *coordinates;

public:
    Geometry (const std::vector<Coordinates>& _coordinates) {
        coordinates = new std::vector<Coordinates>(_coordinates);
    };
    const std::vector<Coordinates> *getCoordinates() const {
        return coordinates;
    }
};

#endif //EX1_GEOMETRY_H
