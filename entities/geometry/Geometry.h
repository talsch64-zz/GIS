
#ifndef EX1_GEOMETRY_H
#define EX1_GEOMETRY_H


#include <vector>
#include <rapidjson/fwd.h>
#include <rapidjson/document.h>
#include "../../GISdefs.h"
#include "Coordinates.h"

class GeometryJsonSerializer;


class Geometry {
protected:
    std::vector<Coordinates> coordinates;
    GeometryJsonSerializer *geometryJsonSerializer;

public:
    explicit Geometry(std::vector<Coordinates> coordinates);

    Geometry() {}

    virtual ~Geometry() = 0;

    std::vector<Coordinates> getCoordinates();

    virtual rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) = 0;
};

#endif //EX1_GEOMETRY_H
