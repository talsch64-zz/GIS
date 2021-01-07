
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
    return getCumulativeSegmentsLength()[points.size() - 1];
}


const size_t PointList::getContainingSegment(Coordinates coordinates) const {
    for (int i = 0; i < points.size() - 1; ++i) {
        if (std::fabs(
                static_cast<double>(getCumulativeSegmentsLength()[i + 1] - getCumulativeSegmentsLength()[i] -
                                    (CoordinatesMath::calculateDistance(points[i], coordinates) +
                                     CoordinatesMath::calculateDistance(coordinates, points[i + 1])))) <=
            DISTANCE_PRECISION) {
            return i;
        }
    }
    return -1;
}

const std::vector<Meters> &PointList::getCumulativeSegmentsLength() const {
    if (!cumulativeSegmentsLength.has_value()) {
        std::vector<Meters> segments;
        segments.push_back(Meters(0));
        for (std::size_t i = 1; i < points.size(); i++) {
            segments.push_back(segments[i - 1] + CoordinatesMath::calculateDistance(points[i - 1], points[i]));
        }
        cumulativeSegmentsLength = std::move(segments);
    }
    return cumulativeSegmentsLength.value();
}


const Meters PointList::getDistanceFromStart(std::size_t segment, const Coordinates &coordinates) {
    if (segment >= points.size() - 1 || segment < 0) {
        //TODO find a better solution
        throw std::runtime_error("invalid segment number");
    }
    Coordinates segmentStart = points[segment];
    Meters distanceFromStart =
            getCumulativeSegmentsLength()[segment] + CoordinatesMath::calculateDistance(segmentStart, coordinates);
    return distanceFromStart;
}


const Meters PointList::getDistanceFromEnd(std::size_t segment, const Coordinates &coordinates) {
    if (segment >= points.size() - 1 || segment < 0) {
        //TODO find a better solution
        throw std::runtime_error("invalid segment number");
    }
    Coordinates segmentEnd = points[segment + 1];
    Meters distanceFromEnd = getLength() - getCumulativeSegmentsLength()[segment + 1] +
                             CoordinatesMath::calculateDistance(coordinates, segmentEnd);
    return distanceFromEnd;
}




