#ifndef EX1_COORDINATESPARSER_H
#define EX1_COORDINATESPARSER_H


#include <rapidjson/fwd.h>
#include <vector>
#include "../GISdefs.h"
#include <rapidjson/document.h>

class CoordinatesParser {
public:
    static Coordinates parse(rapidjson::Value &coordinates);
};

#endif //EX1_COORDINATESPARSER_H
