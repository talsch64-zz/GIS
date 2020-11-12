//
// Created by Tal on 12/11/2020.
//

#ifndef EX1_COORDINATESPARSER_H
#define EX1_COORDINATESPARSER_H


#include <rapidjson/fwd.h>
#include <vector>
#include "../GISdefs.h"
#include <rapidjson/document.h>

class CoordinatesParser {
public:
    static Coordinates parseCoordinates(rapidjson::Value &doc);
};

#endif //EX1_COORDINATESPARSER_H
