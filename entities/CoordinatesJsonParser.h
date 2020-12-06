#ifndef EX1_COORDINATESJSONPARSER_H
#define EX1_COORDINATESJSONPARSER_H


#include <rapidjson/fwd.h>
#include <vector>
#include "../GISNamedTypes.h"
#include <rapidjson/document.h>

/// CoordinatesJsonParser handles coordinates parsing from JSON files

class CoordinatesJsonParser {
public:
    Coordinates parse(rapidjson::Value &coordinates);
};

#endif //EX1_COORDINATESJSONPARSER_H
