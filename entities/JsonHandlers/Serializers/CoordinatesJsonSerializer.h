#ifndef EX1_COORDINATESJSONSERIALIZER_H
#define EX1_COORDINATESJSONSERIALIZER_H


#include <rapidjson/fwd.h>
#include <rapidjson/document.h>
#include "../../../Common/GISNamedTypes.h"

/// CoordinatesJsonSerializer handles coordinates serialization into JSON format
class CoordinatesJsonSerializer {
public:
    rapidjson::Value toJson(Coordinates coordinates, rapidjson::Document::AllocatorType &allocator);
};


#endif //EX1_COORDINATESJSONSERIALIZER_H
