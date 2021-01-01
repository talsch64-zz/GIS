
#include <memory>
#include "GeometryJsonParser.h"
#include "../../geometry/Point.h"
#include "../../geometry/PointList.h"

std::unique_ptr<Circle> GeometryJsonParser::parsePOIGeometry(rapidjson::Value &doc) {
    if (!doc.HasMember("geometry") || !doc["geometry"].IsObject()) {
        throw std::runtime_error("JSON entity doesn't contain geometry");
    }
    rapidjson::Value geometry = doc["geometry"].GetObject();
    if (!geometry.HasMember("type") || !geometry["type"].IsString()) {
        throw std::runtime_error("Geomtry JSON doesn't contain type");
    }
    std::string type = geometry["type"].GetString();
    if (type == "Circle") {
        return parseCircle(geometry);

    } else {
        throw std::runtime_error("Unsupported geometry type: " + type);
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

std::unique_ptr<PointList> GeometryJsonParser::parseWayGeometry(rapidjson::Value &doc, Coordinates &fromCoord, Coordinates &toCoord) {
//    std::unique_ptr<PointList> pointList (new PointList());
    std::vector<Coordinates> points;
    points.push_back(fromCoord);
    if (doc.HasMember("curves") && doc["curves"].IsArray()) {
        for (auto &coordinates : doc["curves"].GetArray()) {
            points.push_back(coordinatesJsonParser.parse(coordinates));
        }
    }
    points.push_back(toCoord);
    return std::make_unique<PointList>(points);
}

std::unique_ptr<Point> GeometryJsonParser::parseJunctionGeometry(rapidjson::Value &doc) {
    if (!doc.HasMember("coordinates") || !doc["coordinates"].IsArray())  {
        throw std::runtime_error("Invalid coordinates in JSON");
    }
    Coordinates coordinates = coordinatesJsonParser.parse(doc["coordinates"]);
//    std::unique_ptr<Point> point(new Point(coordinates));
    return std::make_unique<Point>(coordinates);;
}
