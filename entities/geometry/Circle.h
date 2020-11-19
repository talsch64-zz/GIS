#ifndef EX1_CIRCLE_H
#define EX1_CIRCLE_H

#include "Geometry.h"
#include <rapidjson/document.h>

class Circle : public Geometry {
    Meters radius;
public:
    Circle(const Coordinates &coordinates, const Meters radius) : Geometry(coordinates), radius(radius) {}

    const Meters &getRadius() const {
        return radius;
    }

    rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) override;

    Coordinates getCenter();
};

#endif //EX1_CIRCLE_H
