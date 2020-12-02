#include "TopologicalSearch.h"

#include <utility>
#include "../entities/geometry/CoordinatesMath.h"

std::vector<Coordinates>
TopologicalSearch::searchCircleInGrid(const Grid &grid, const Coordinates &center, Meters radius) const {
    std::vector<Coordinates> gridCells;
    Coordinates leftCoord = CoordinatesMath::CoordinatesByBearingAndDistance(center, radius, -90);
    Coordinates topCoord = CoordinatesMath::CoordinatesByBearingAndDistance(center, radius, 0);
    Coordinates rightCoord = CoordinatesMath::CoordinatesByBearingAndDistance(center, radius, 90);
    Coordinates bottomCoord = CoordinatesMath::CoordinatesByBearingAndDistance(center, radius, 180);

    for (double lat = bottomCoord.latitude(); lat <= topCoord.latitude(); lat += grid.precision) {
        for (double lon = leftCoord.longitude(); lon <= rightCoord.longitude(); lon += grid.precision) {
            Coordinates coord = grid.truncateCoordinates(Coordinates(Longitude(lon), Latitude(lat)));
            gridCells.emplace_back(coord);
        }
    }
    return gridCells;
}

bool TopologicalSearch::isInCircle(const Coordinates &center, Meters radius, const Circle &entityGeometry) const {
    Meters distanceBetweenCenters = CoordinatesMath::calculateDistance(center, entityGeometry.getCenter());
    bool inCircle = distanceBetweenCenters <= radius + entityGeometry.getRadius();
    return inCircle;
}

bool TopologicalSearch::isInCircle(const Coordinates &center, Meters radius, const PointList &entityGeometry) const {
    std::vector<Coordinates> coords = entityGeometry.getPoints();
    auto coordsIter = coords.begin();
    Coordinates prevCoord = *coordsIter;
    for (coordsIter = next(coordsIter); coordsIter < coords.end(); coordsIter++) {
        Coordinates coord = *coordsIter;
        Meters distance = CoordinatesMath::distanceFromSegment(prevCoord, coord, center);
        if (distance <= radius) {
            return true;
        }
        prevCoord = coord;
    }
    return false;
}

bool TopologicalSearch::isInCircle(const Coordinates &center, Meters radius, const Point &entityGeometry) const {
    Meters distance = CoordinatesMath::calculateDistance(center, entityGeometry.getCoordinates());
    return distance <= radius;
}


