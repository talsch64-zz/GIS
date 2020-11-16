
#ifndef EX1_GEOMETRY_H
#define EX1_GEOMETRY_H


#include <vector>
#include "../../GISdefs.h"

class Geometry {
    const std::vector<Coordinates> *coordinates = nullptr;

public:
    Geometry(const std::vector<Coordinates> &coordinates);
    Geometry() {}
    virtual ~Geometry() = 0;
};

#endif //EX1_GEOMETRY_H
