#include "TopologicalSearch.h"

#include <utility>
#include "../entities/geometry/CoordinatesMath.h"

TopologicalSearch::TopologicalSearch(std::shared_ptr<Grid> grid) : grid(std::move(grid)) {}

std::vector<Coordinates> TopologicalSearch::searchCircleInGrid(const Coordinates &center, Meters radius) {
    std::vector<Coordinates> gridCells;
    Coordinates leftCoord = CoordinatesMath::calculateCoordinatesByDistanceAndBearing(center, radius, -90);
    Coordinates topCoord = CoordinatesMath::calculateCoordinatesByDistanceAndBearing(center, radius, 0);
    Coordinates rightCoord = CoordinatesMath::calculateCoordinatesByDistanceAndBearing(center, radius, 90);
    Coordinates bottomCoord = CoordinatesMath::calculateCoordinatesByDistanceAndBearing(center, radius, 180);

    for (double lat = bottomCoord.latitude(); lat <= topCoord.latitude(); lat += grid->precision) {
        for (double lon = leftCoord.longitude(); lon <= rightCoord.longitude(); lon += grid->precision) {
            gridCells.emplace_back(Coordinates(Longitude(lon), Latitude(lat)));
        }
    }
    return gridCells;
}


