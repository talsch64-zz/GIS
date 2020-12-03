
#include "PointList.h"

#include <utility>
#include "../JsonHandlers/Serializers/GeometryJsonSerializer.h"
#include "../search/Grid.h"
#include "CoordinatesMath.h"

PointList::PointList(std::vector<Coordinates> &points) : points(std::move(points)), Geometry() {}

const std::vector<Coordinates> &PointList::getPoints() const {
    return points;
}

rapidjson::Value PointList::toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) {
    return geometryJsonSerializer->toJson(this, allocator);
}

std::vector<Grid::GridCell> PointList::getGridCells(const Grid *grid) {
    return grid->getGeometryGridCells(*this);
}

bool PointList::isInCircle(const TopologicalSearch *topologicalSearch, const Coordinates &center, Meters radius) const {
    return topologicalSearch->isInCircle(center, radius, *this);
}

Coordinates PointList::getClosestPoint(const Coordinates &coordinates) const {
    std::vector<Coordinates> coords = getPoints();
//  initialize dummy pair;
    std::pair<Coordinates, Meters> closestPair{Coordinates(Longitude(0), Latitude(0)), INFINITY};
    for (int i = 0; i < coords.size() - 1; ++i) {
        std::pair<Coordinates, Meters> nextPair = CoordinatesMath::closestPointOnSegmentAndDistance(coordinates,
                                                                                                    coords[i],
                                                                                                    coords[i + 1]);
        if (closestPair.second > nextPair.second) {
            closestPair = nextPair;
        }
    }
    return closestPair.first;
}



