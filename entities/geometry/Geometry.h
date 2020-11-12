
#ifndef EX1_GEOMETRY_H
#define EX1_GEOMETRY_H


#include <vector>
#include "../../GISdefs.h"

class Geometry {
    const std::vector<Coordinates> *coordinates = nullptr;

public:
    Geometry(const std::vector<Coordinates> &coordinates);
    Geometry() {}

    const std::vector<Coordinates> *getCoordinates() const;
};

#endif //EX1_GEOMETRY_H
