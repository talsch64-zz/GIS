#include "GeometryJsonSerializer.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/Circle.h"
#include "../../geometry/Point.h"
#include "../../geometry/PointList.h"

rapidjson::Value GeometryJsonSerializer::toJson(Circle *geometry, rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value json;
    json.SetObject();

    rapidjson::Value type;
    type.SetString("Circle");
    json.AddMember("type", type, allocator);

    rapidjson::Value coordinates = coordinatesJsonSerializer.toJson(geometry->getCenter(), allocator);
    json.AddMember("coordinates", coordinates, allocator);

    rapidjson::Value radius;
    radius.SetDouble(geometry->getRadius());
    json.AddMember("radius", radius, allocator);

    return json;
}

rapidjson::Value GeometryJsonSerializer::toJson(Point *geometry, rapidjson::Document::AllocatorType &allocator) {
    return coordinatesJsonSerializer.toJson(geometry->getCoordinates(), allocator);
}

rapidjson::Value GeometryJsonSerializer::toJson(PointList *geometry, rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value curves;
    curves.SetArray();
    std::vector<Coordinates> points = geometry->getPoints();
//    skipping first and last coordinates
    for(int i = 1; i < points.size()-1; i++) {
        rapidjson::Value coordVal = coordinatesJsonSerializer.toJson(points[i], allocator);
        curves.PushBack(coordVal, allocator);
    }
    return curves;
}