#include "TopologicalSearch.h"

#include <utility>
#include "../entities/geometry/CoordinatesMath.h"

std::vector<Coordinates>
TopologicalSearch::searchCircleInGrid(const Grid &grid, const Coordinates &center, Meters radius) const {
    std::vector<Coordinates> gridCells;
    Coordinates leftCoord = CoordinatesMath::calculateCoordinateByDistance(center, radius, -90);
    Coordinates topCoord = CoordinatesMath::calculateCoordinateByDistance(center, radius, 0);
    Coordinates rightCoord = CoordinatesMath::calculateCoordinateByDistance(center, radius, 90);
    Coordinates bottomCoord = CoordinatesMath::calculateCoordinateByDistance(center, radius, 180);

    for (double lat = bottomCoord.latitude(); lat <= topCoord.latitude(); lat += grid.precision) {
        for (double lon = leftCoord.longitude(); lon <= rightCoord.longitude(); lon += grid.precision) {
            gridCells.emplace_back(Coordinates(Longitude(lon), Latitude(lat)));
        }
    }
    return gridCells;
}

bool TopologicalSearch::isInCircle(const Coordinates &center, Meters radius, const Circle &entityGeometry) const {
    Meters distanceBetweenCenters = CoordinatesMath::calculateDistance(center, entityGeometry.getCenter());
    bool inCircle = distanceBetweenCenters <= radius + entityGeometry.getRadius();
    return inCircle;
}


