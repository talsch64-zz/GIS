//
// Created by Tal on 12/11/2020.
//

#include <stdexcept>
#include "CoordinatesParser.h"


Coordinates CoordinatesParser::parseCoordinates(rapidjson::Value &doc) {
    if (!doc.HasMember("coordinates") || !doc["coordinates"].IsArray() || doc["coordinates"].Size() != 2 ||
            !doc["coordinates"][0].IsNumber() || !doc["coordinates"][1].IsNumber())  {
        throw std::runtime_error("Invalid coordinate in JSON");
    }

    Coordinates coord(doc["coordinates"][0].GetDouble(), doc["coordinates"][1].GetDouble());
    return coord;
}


