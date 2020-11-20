
#include <memory>
#include "GeometryJsonParser.h"
#include "Point.h"
#include "PointList.h"

std::unique_ptr<Geometry> GeometryJsonParser::parsePOIGeometry(rapidjson::Value &doc) {
    if (!doc.HasMember("geometry") || !doc["geometry"].IsObject()) {
        throw std::runtime_error("JSON entity doesn't contain geometry");
    }
    doc = doc["geometry"];
    if (!doc.HasMember("type") || !doc["type"].IsString()) {
        throw std::runtime_error("Geomtry JSON doesn't contain type");
    }
    std::string type = doc["type"].GetString();
    if (type == "Circle") {
        return parseCircle(doc);
//    } else if (type == "Polygon") {
//        return parsePolygon(doc);
    } else {
        throw std::runtime_error("Unsupported geometry type");
    }
}

std::unique_ptr<Circle> GeometryJsonParser::parseCircle(rapidjson::Value &doc) {
    if (!doc.HasMember("coordinates") || !doc["coordinates"].IsArray())  {
        throw std::runtime_error("Invalid coordinates in JSON");
    }
    Coordinates coordinates = coordinatesJsonParser.parse(doc["coordinates"]);
    if (!doc.HasMember("radius") || !doc["radius"].IsNumber()) {
        throw std::runtime_error("Circle JSON doesn't contain radius");
    }
    Meters radius(doc["radius"].GetDouble());
    std::unique_ptr<Circle> circle(new Circle(coordinates, radius));
    return circle;
}

std::unique_ptr<Geometry> GeometryJsonParser::parseWayGeometry(rapidjson::Value &doc) {
    std::unique_ptr<PointList> pointList;
//    insert dummy point to be replaced later by "to" junction coordinates
    pointList->addPoint(Coordinates(Longitude(0), Latitude(0)));
    if (doc.HasMember("curves") && doc["curves"].IsArray()) {
        for (auto &coordinates : doc["curves"].GetArray()) {
            pointList->addPoint(coordinatesJsonParser.parse(coordinates));
        }
    }
    return pointList;
}

std::unique_ptr<Geometry> GeometryJsonParser::parseJunctionGeometry(rapidjson::Value &doc) {
    if (!doc.HasMember("coordinates") || !doc["coordinates"].IsArray())  {
        throw std::runtime_error("Invalid coordinates in JSON");
    }
    Coordinates coordinates = coordinatesJsonParser.parse(doc["coordinates"]);
    std::unique_ptr<Point> point(new Point(coordinates));
    return point;
}


std::unique_ptr<Polygon> parsePolygon(rapidjson::Value &value) {
    return nullptr;
}