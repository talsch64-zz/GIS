//
// Created by Tal on 19/11/2020.
//

#ifndef EX1_POINTLIST_H
#define EX1_POINTLIST_H
#include "Point.h"
#include "Geometry.h"

class PointList: public Geometry {
    std::vector<Point> points;

public:
    const std::vector<Point> &getPoints() const;

    PointList(std::vector<Point> points);

    void addPoint(Point p);

    rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) override;
};
#endif //EX1_POINTLIST_H
