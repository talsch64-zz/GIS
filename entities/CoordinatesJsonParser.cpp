//
// Created by Tal on 12/11/2020.
//

#include <stdexcept>
#include "CoordinatesJsonParser.h"


Coordinates CoordinatesJsonParser::parse(rapidjson::Value &coordinates) {
    if (coordinates.Size() != 2 || !coordinates[0].IsNumber() || !coordinates[1].IsNumber()) {
        throw std::runtime_error("Invalid coordinate in JSON");
    }
    return Coordinates(Longitude(coordinates[0].GetDouble()), Latitude(coordinates[1].GetDouble()));
}


