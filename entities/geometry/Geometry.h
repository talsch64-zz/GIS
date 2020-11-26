
#ifndef EX1_GEOMETRY_H
#define EX1_GEOMETRY_H


#include <vector>
#include <rapidjson/fwd.h>
#include <rapidjson/document.h>
#include "../../GISdefs.h"

class Grid;

class GeometryJsonSerializer;

class Geometry {

protected:
    GeometryJsonSerializer *geometryJsonSerializer;

    Geometry();

public:

    virtual ~Geometry();

    virtual rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) = 0;

    virtual std::vector<Coordinates> getGridCells(const Grid *grid) = 0;

    virtual Coordinates getClosestPoint(const Coordinates &coordinates) = 0;
};

#endif //EX1_GEOMETRY_H
