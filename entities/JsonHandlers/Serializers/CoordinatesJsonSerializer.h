#ifndef EX1_COORDINATESJSONSERIALIZER_H
#define EX1_COORDINATESJSONSERIALIZER_H


#include <rapidjson/fwd.h>
#include <rapidjson/document.h>
#include "../../../GISNamedTypes.h"

class CoordinatesJsonSerializer {
public:
    rapidjson::Value toJson(Coordinates coordinates, rapidjson::Document::AllocatorType &allocator);
};


#endif //EX1_COORDINATESJSONSERIALIZER_H
