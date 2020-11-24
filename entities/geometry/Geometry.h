
#ifndef EX1_GEOMETRY_H
#define EX1_GEOMETRY_H


#include <vector>
#include <rapidjson/fwd.h>
#include <rapidjson/document.h>
#include "../../GISdefs.h"

class GeometryJsonSerializer;


class Geometry {
    std::string type;
//    TODO set type to be enum
protected:
    GeometryJsonSerializer *geometryJsonSerializer;
    Geometry(std::string type);

public:
    enum type {POINT, POINTLIST, CIRCLE};

    const std::string &getType() const;

    virtual ~Geometry();

    virtual rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) = 0;
};

#endif //EX1_GEOMETRY_H
