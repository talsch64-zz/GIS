//
// Created by Tal on 19/11/2020.
//

#ifndef EX1_POINTLIST_H
#define EX1_POINTLIST_H

#include "Geometry.h"
#include <rapidjson/document.h>

//TODO test
#define DISTANCE_PRECISION 5

/// Geometry derived class that represents continuous segments.
/// The segments are represented as a vector of Coordinates where each segment is defined by two consecutive Coordinates inside the vector
class PointList : public Geometry {
    std::vector<Coordinates> points;
    mutable std::optional<std::vector<Meters>> cumulativeSegmentsLength; // vector that holds the cumulative length of the segments

public:

    explicit PointList(std::vector<Coordinates> &points);

    const std::vector<Coordinates> &getPoints() const;

    rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) const override;

    std::vector<Coordinates> getGridCells(const Grid *grid) const override;

    Coordinates getClosestPoint(const Coordinates &coordinates) const override;

    bool
    isInCircle(const TopologicalSearch *topologicalSearch, const Coordinates &center, Meters radius) const override;

    Meters getLength() const;

    /**
    *
    * @param coordinates
    * @return the number of segment containing coordinates if exists, else -1
    */
    const size_t getContainingSegment(Coordinates coordinates) const;


    /**
     *
     * @return vector of cumulative length of all the segments.
     */
    const std::vector<Meters> &getCumulativeSegmentsLength() const;


};

#endif //EX1_POINTLIST_H
