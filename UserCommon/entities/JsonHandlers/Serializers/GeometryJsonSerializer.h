#ifndef EX1_GEOMETRYJSONSERIALIZER_H
#define EX1_GEOMETRYJSONSERIALIZER_H


#include <rapidjson/fwd.h>
#include <rapidjson/document.h>
#include "CoordinatesJsonSerializer.h"
#include "../../geometry/Point.h"
#include "../../geometry/PointList.h"

class Circle;

/// GeometryJsonSerializer handles the serialization of the different geometries into JSON format
class GeometryJsonSerializer {
    CoordinatesJsonSerializer coordinatesJsonSerializer;

public:
    rapidjson::Value toJson(const Circle *geometry, rapidjson::Document::AllocatorType &allocator);

    rapidjson::Value toJson(const Point *geometry, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator);

    rapidjson::Value toJson(const PointList *geometry, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator);
};


#endif //EX1_GEOMETRYJSONSERIALIZER_H
