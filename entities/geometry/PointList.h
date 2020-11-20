//
// Created by Tal on 19/11/2020.
//

#ifndef EX1_POINTLIST_H
#define EX1_POINTLIST_H
#include "Point.h"
#include "Geometry.h"

class PointList: public Geometry {
    std::vector<Coordinates> points;

public:

    PointList(const std::vector<Coordinates> &points);

    const std::vector<Coordinates> &getPoints() const;

    void addPoint(Coordinates coord);

    rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) override;
};
#endif //EX1_POINTLIST_H
