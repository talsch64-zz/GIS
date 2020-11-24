
#ifndef EX1_GEOMETRY_H
#define EX1_GEOMETRY_H


#include <vector>
#include <rapidjson/fwd.h>
#include <rapidjson/document.h>
#include "../../GISdefs.h"

class GeometryJsonSerializer;

class Geometry {
protected:
    GeometryJsonSerializer *geometryJsonSerializer;
    Geometry();

public:

    virtual ~Geometry();

    virtual rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) = 0;
};

#endif //EX1_GEOMETRY_H
