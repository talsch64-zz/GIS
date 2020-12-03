#ifndef EX1_TOPOLOGICALSEARCH_H
#define EX1_TOPOLOGICALSEARCH_H


#include <vector>
#include <memory>
#include "../GISdefs.h"
#include "Grid.h"
#include "../entities/geometry/Circle.h"
#include "../entities/geometry/PointList.h"
#include "../entities/geometry/Point.h"

class TopologicalSearch {
public:
    std::vector<Coordinates> searchCircleInGrid(const Grid &grid, const Coordinates &center, Meters radius) const;

    bool isInCircle(const Coordinates &center, Meters radius, const Circle &entityGeometry) const;

    bool isInCircle(const Coordinates &center, Meters radius, const PointList &entityGeometry) const;

    bool isInCircle(const Coordinates &center, Meters radius, const Point &entityGeometry) const;

};


#endif //EX1_TOPOLOGICALSEARCH_H
