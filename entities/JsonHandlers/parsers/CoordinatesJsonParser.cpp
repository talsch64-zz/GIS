#include <stdexcept>
#include "CoordinatesJsonParser.h"

Coordinates CoordinatesJsonParser::parse(rapidjson::Value &coordinates) {
    if (!coordinates.IsArray() || coordinates.Size() != 2 || !coordinates[0].IsNumber() || !coordinates[1].IsNumber()) {
        throw std::runtime_error("Invalid coordinate in JSON");
    }
    if (coordinates[0].GetDouble() > 180 || coordinates[0].GetDouble() <= -180 || abs(coordinates[1].GetDouble()) > 90) {
        throw std::runtime_error("Invalid coordinate in JSON");
    }
    return Coordinates(Longitude(coordinates[0].GetDouble()), Latitude(coordinates[1].GetDouble()));
}


