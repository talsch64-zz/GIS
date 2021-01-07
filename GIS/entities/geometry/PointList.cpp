
#include "PointList.h"
#include <limits.h>
#include <utility>
#include <iostream>
#include "../JsonHandlers/Serializers/GeometryJsonSerializer.h"
#include "../../search/Grid.h"
#include "../../../Common/CoordinatesMath.h"

PointList::PointList(std::vector<Coordinates> &points) : Geometry(), points(std::move(points)) {}

const std::vector<Coordinates> &PointList::getPoints() const {
    return points;
}

rapidjson::Value PointList::toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    return geometryJsonSerializer->toJson(this, allocator);
}

std::vector<Grid::GridCell> PointList::getGridCells(const Grid *grid) const {
    return grid->getGeometryGridCells(*this);
}

bool PointList::isInCircle(const TopologicalSearch *topologicalSearch, const Coordinates &center, Meters radius) const {
    return topologicalSearch->isInCircle(center, radius, *this);
}

Coordinates PointList::getClosestPoint(const Coordinates &coordinates) const {
    std::vector<Coordinates> coords = getPoints();
//  initialize dummy pair;
    std::pair<Coordinates, Meters> closestPair{Coordinates(Longitude(0), Latitude(0)), INT_MAX};
    for (int i = 0; i < (int) coords.size() - 1; ++i) {
        std::pair<Coordinates, Meters> nextPair = CoordinatesMath::closestPointOnSegmentAndDistance(coordinates,
                                                                                                    coords[i],
                                                                                                    coords[i + 1]);
        if (closestPair.second > nextPair.second) {
            closestPair = nextPair;
        }
    }
    return closestPair.first;
}

Meters PointList::getLength() const {
    Meters length(0);
    for (size_t i = 1; i < points.size(); i++) {
        length += CoordinatesMath::calculateDistance(points[i - 1], points[i]);
    }
    return length;
}


