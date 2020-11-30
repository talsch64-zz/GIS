#include <stdexcept>
#include "CoordinatesJsonParser.h"

//TODO reject coordinates the exceeds 180/90
Coordinates CoordinatesJsonParser::parse(rapidjson::Value &coordinates) {
    if (!coordinates.IsArray() || coordinates.Size() != 2 || !coordinates[0].IsNumber() || !coordinates[1].IsNumber()) {
        throw std::runtime_error("Invalid coordinate in JSON");
    }
    return Coordinates(Longitude(coordinates[0].GetDouble()), Latitude(coordinates[1].GetDouble()));
}


