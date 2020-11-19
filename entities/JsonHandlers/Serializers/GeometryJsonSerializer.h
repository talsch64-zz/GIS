#ifndef EX1_GEOMETRYJSONSERIALIZER_H
#define EX1_GEOMETRYJSONSERIALIZER_H


#include <rapidjson/fwd.h>
#include <rapidjson/document.h>
#include "CoordinatesJsonSerializer.h"

class Circle;

class GeometryJsonSerializer {
    CoordinatesJsonSerializer coordinatesJsonSerializer;

public:
    rapidjson::Value toJson(Circle *geometry, rapidjson::Document::AllocatorType &allocator);
};


#endif //EX1_GEOMETRYJSONSERIALIZER_H
