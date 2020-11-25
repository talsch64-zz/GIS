#ifndef EX1_CIRCLE_H
#define EX1_CIRCLE_H

#include "Geometry.h"
#include <rapidjson/document.h>

class Grid;

class Circle : public Geometry {
    Meters radius;
    Coordinates coordinates;
public:
    Circle(const Coordinates &coordinates, const Meters radius);

    const Meters &getRadius() const {
        return radius;
    }

    rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) override;

    Coordinates getCenter() const;

    std::vector<Coordinates> getGridCells(const Grid *grid) override;

    bool isInCircle(const TopologicalSearch *topologicalSearch, const Coordinates &searchCenter, Meters searchRadius) const override;
};

#endif //EX1_CIRCLE_H
