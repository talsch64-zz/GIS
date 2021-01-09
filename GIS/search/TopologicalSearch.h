#ifndef EX1_TOPOLOGICALSEARCH_H
#define EX1_TOPOLOGICALSEARCH_H


#include <vector>
#include <memory>
#include "../../Common/GISNamedTypes.h"
#include "Grid.h"
#include "../UserCommon/entities/geometry/Circle.h"
#include "../UserCommon/entities/geometry/PointList.h"
#include "../UserCommon/entities/geometry/Point.h"




/**
 * TopologicalSearch handles Grid searches
 * Operations supported:
 *   * Search for all the grid-cells inside the perimeter of a given circle
 *   * Check if a given geometry runs through or overlaps a given circle.
 */

class TopologicalSearch {
    static constexpr Meters SAFETY_MARGIN = Meters(500);
public:
    std::vector<Coordinates> searchCircleInGrid(const Grid &grid, const Coordinates &center, Meters radius) const;

    bool isInCircle(const Coordinates &center, Meters radius, const Circle &entityGeometry) const;

    bool isInCircle(const Coordinates &center, Meters radius, const PointList &entityGeometry) const;

    bool isInCircle(const Coordinates &center, Meters radius, const Point &entityGeometry) const;

};


#endif //EX1_TOPOLOGICALSEARCH_H
