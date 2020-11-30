//
// Created by Tal on 19/11/2020.
//

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
    return false;
}

Coordinates PointList::getClosestPoint(const Coordinates &coordinates) {
    std::vector<Coordinates> coords = getPoints();
//  initialize dummy pair;
    std::pair<Meters, Coordinates> closestPair {INFINITY, Coordinates(Longitude(0), Latitude(0))};
    for(int i = 0; i < coords.size()-1; ++i) {
        std::pair<Meters, Coordinates> nextPair = CoordinatesMath::closestPointOnSegmentAndDistance(coords[i],coordinates);
        if (closestPair.first > nextPair.first) {
            closestPair = nextPair;
        }
    }
    return closestPair.second;
}



