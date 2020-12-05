
#ifndef EX1_GEOMETRY_H
#define EX1_GEOMETRY_H


#include <vector>
#include <rapidjson/fwd.h>
#include <rapidjson/document.h>
#include "../../GISNamedTypes.h"
#include <memory>

class TopologicalSearch;

class Grid;

class GeometryJsonSerializer;

class Geometry {

protected:
    std::shared_ptr<GeometryJsonSerializer> geometryJsonSerializer;

    Geometry();

public:

    virtual ~Geometry();

    virtual rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) = 0;

    virtual std::vector<Coordinates> getGridCells(const Grid *grid) = 0;

    virtual Coordinates getClosestPoint(const Coordinates &coordinates) const = 0;

    virtual bool
    isInCircle(const TopologicalSearch *topologicalSearch, const Coordinates &center, Meters radius) const = 0;
};

#endif //EX1_GEOMETRY_H
