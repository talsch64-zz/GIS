#ifndef EX1_TOPOLOGICALSEARCH_H
#define EX1_TOPOLOGICALSEARCH_H


#include <vector>
#include <memory>
#include "../GISdefs.h"
#include "Grid.h"
#include "../entities/geometry/Circle.h"

class TopologicalSearch {
public:
    std::vector<Coordinates> searchCircleInGrid(const Grid &grid, const Coordinates &center, Meters radius) const;

    bool isInCircle(const Coordinates &center, Meters radius, const Circle &entityGeometry);
};


#endif //EX1_TOPOLOGICALSEARCH_H
