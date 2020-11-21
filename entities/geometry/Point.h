//
// Created by Tal on 19/11/2020.
//

#ifndef EX1_POINT_H
#define EX1_POINT_H
#include "Geometry.h"
#include "../../GISdefs.h"


class Point: public Geometry {
    Coordinates coordinates;
public:
    Point(const Coordinates &coordinates);

    const Coordinates &getCoordinates() const;

    rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) override;
};
#endif //EX1_POINT_H