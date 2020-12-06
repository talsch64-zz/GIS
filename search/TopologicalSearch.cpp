#include "TopologicalSearch.h"

#include <utility>
#include <queue>
#include "../CoordinatesMath.h"

std::vector<Coordinates>
TopologicalSearch::searchCircleInGrid(const Grid &grid, const Coordinates &center, Meters radius) const {
    radius = Meters(radius + SAFETY_MARGIN);
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
    Coordinates closest = entityGeometry.getClosestPoint(center);
    return CoordinatesMath::calculateDistance(center, closest) <= radius;
}

bool TopologicalSearch::isInCircle(const Coordinates &center, Meters radius, const Point &entityGeometry) const {
    Meters distance = CoordinatesMath::calculateDistance(center, entityGeometry.getCoordinates());
    return distance <= radius;
}


