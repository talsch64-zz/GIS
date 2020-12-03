#include "TopologicalSearch.h"

#include <utility>
#include <set>
#include <queue>
#include "../entities/geometry/CoordinatesMath.h"

std::vector<Coordinates>
TopologicalSearch::searchCircleInGrid(const Grid &grid, const Coordinates &center, Meters radius) const {
    std::vector<Coordinates> cellsInCircle;
    std::queue<Coordinates> queue;
    queue.push(grid.truncateCoordinates(center));
    std::unordered_set<Coordinates> searchedCoord;
    while (!queue.empty()) {
        Coordinates coord = queue.front();
        queue.pop();
        cellsInCircle.push_back(coord);
        std::vector<Coordinates> neighbors = grid.getCellNeighbors(coord);
        for (Coordinates neighbor: neighbors) {
            neighbor = grid.truncateCoordinates(neighbor);
            if (searchedCoord.find(neighbor) == searchedCoord.end()) {
                searchedCoord.insert(neighbor);
                Meters distance = CoordinatesMath::calculateDistance(center, neighbor);
                if (distance <= radius) {
                    queue.push(neighbor);
                } else {
                    cellsInCircle.push_back(neighbor);
                }
            }
        }
    }
    return cellsInCircle;
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
        Meters distance = CoordinatesMath::distanceFromSegment(center, prevCoord, coord);
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


