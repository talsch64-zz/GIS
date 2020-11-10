
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
    std::vector<Coordinates> coordinates;
    getCoordinates(doc, coordinates);
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

void GeometryJsonParser::getCoordinates(rapidjson::Value &doc, std::vector<Coordinates> &coordinates) {
    if (!doc.HasMember("coordinates") || !doc["coordinates"].IsArray() || doc["coordinates"].Size() == 0) {
        throw std::runtime_error("Invalid circle JSON");
    }
    if (!doc["coordinates"].IsArray()) {
        throw std::runtime_error("Invalid coordinates JSON");
    }

    for (auto &coordinateJSON: doc["coordinates"].GetArray()) {
        if (!coordinateJSON.IsArray() || coordinateJSON.Size() != 2 || !coordinateJSON[0].IsNumber() ||
            !coordinateJSON[1].IsNumber()) {
            throw std::runtime_error("Invalid coordinate in JSON");
        }
        Coordinates coord(coordinateJSON[0].GetDouble(), coordinateJSON[1].GetDouble());
        coordinates.push_back(coord);
    }
}