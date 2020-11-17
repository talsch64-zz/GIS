//
// Created by Tal on 12/11/2020.
//

#ifndef EX1_COORDINATESPARSER_H
#define EX1_COORDINATESPARSER_H


#include <rapidjson/fwd.h>
#include <vector>
#include "../GISdefs.h"
#include "geometry/Coordinates.h"
#include <rapidjson/document.h>

class CoordinatesParser {
public:
    static Coordinates parse(rapidjson::Value &coordinates);
};

#endif //EX1_COORDINATESPARSER_H
