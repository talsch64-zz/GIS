#include "GeometryJsonSerializer.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/Circle.h"

rapidjson::Value GeometryJsonSerializer::toJson(Circle *geometry, rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value json;
    json.SetObject();

    rapidjson::Value type;
    type.SetString("Circle");
    json.AddMember("type", type, allocator);

    rapidjson::Value coordinates = coordinatesJsonSerializer.toJson(geometry->getCenter(), allocator);
    json.AddMember("coordinates", coordinates, allocator);

    return json;
}