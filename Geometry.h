
#ifndef EX1_GEOMETRY_H
#define EX1_GEOMETRY_H


#include <vector>
#include "GIS.h"

class Geometry {
    const std::vector<Coordinates> *coordinates;

public:
    Geometry(const std::vector<Coordinates> &coordinates);

    const std::vector<Coordinates> *getCoordinates() const;
};

#endif //EX1_GEOMETRY_H
