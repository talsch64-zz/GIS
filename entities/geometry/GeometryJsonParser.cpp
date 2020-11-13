
#include "GeometryJsonParser.h"


Geometry GeometryJsonParser::parseGeometry(rapidjson::Value &doc) {
    if (!doc.HasMember("type") || !doc["type"].IsString()) {
        throw std::runtime_error("Geomtry JSON doesn't contain type");
    }
    std::string type = doc["type"].GetString();
    if (type == "Circle") {
        return parseCircle(doc);
    } else if (type == "Polygon") {
        return parsePolygon(doc);
    } else {
        throw std::runtime_error("Unsupported geometry type");
    }
}

Circle GeometryJsonParser::parseCircle(rapidjson::Value &doc) {
    std::vector<Coordinates> coordinates = parseCoordinates(doc);
    if (!doc.HasMember("radius") || !doc["radius"].IsNumber()) {
        throw std::runtime_error("Circle JSON doesn't contain radius");
    }
    Meters radius(doc["radius"].GetDouble());
    Circle circle(coordinates, radius);
    return circle;
}

Geometry GeometryJsonParser::parsePolygon(rapidjson::Value &value) {
    return Polygon(std::vector<Coordinates>());
}


std::vector<Coordinates> GeometryJsonParser::parseCoordinates(rapidjson::Value &doc) {
    std::vector<Coordinates > coordinates;
    if (!doc.HasMember("coordinates") || !doc["coordinates"].IsArray())  {
        throw std::runtime_error("Invalid coordinates in JSON");
    }
    coordinates.push_back(CoordinatesParser::parse(doc["coordinates"]));
    return coordinates;
}