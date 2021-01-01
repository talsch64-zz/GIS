
#ifndef EX1_GEOMETRY_H
#define EX1_GEOMETRY_H


#include <vector>
#include <rapidjson/fwd.h>
#include <rapidjson/document.h>
#include "../../Common/GISNamedTypes.h"
#include <memory>

class TopologicalSearch;

class Grid;

class GeometryJsonSerializer;


/// Geometry abstract class
/// Each Entity has a unique Geometry
/// Operations supported:
/// * Get the closest Coordinates on the geometry's perimeter from the given coordinates
/// * Get all the grid cell that overllap the geometry's perimeter
/// * JSON serialization
class Geometry {

protected:
    std::shared_ptr<GeometryJsonSerializer> geometryJsonSerializer;

    Geometry();

public:

    virtual ~Geometry();

    virtual rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) const = 0;

    virtual std::vector<Coordinates> getGridCells(const Grid *grid) const = 0;

    /* returns the closest Coordinates on the geometry's perimeter from the given coordinates */
    virtual Coordinates getClosestPoint(const Coordinates &coordinates) const = 0;

    virtual bool
    isInCircle(const TopologicalSearch *topologicalSearch, const Coordinates &center, Meters radius) const = 0;
};

#endif //EX1_GEOMETRY_H
