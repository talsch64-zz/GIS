//
// Created by Tal on 19/11/2020.
//

#ifndef EX1_POINTLIST_H
#define EX1_POINTLIST_H
#include "Geometry.h"
#include <rapidjson/document.h>


/// Geometry derived class that represents continuous segments.
/// The segments are represented as a vector of Coordinates where each segment is defined by two consecutive Coordinates inside the vector
class PointList: public Geometry {
    std::vector<Coordinates> points;

public:

    explicit PointList(std::vector<Coordinates> &points);

    const std::vector<Coordinates> &getPoints() const;

    rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) override;

    std::vector<Coordinates> getGridCells(const Grid *grid) override;

    Coordinates getClosestPoint(const Coordinates &coordinates) const override;
    
    bool isInCircle(const TopologicalSearch *topologicalSearch, const Coordinates &center, Meters radius) const override;
};
#endif //EX1_POINTLIST_H
