#include "CoordinatesJsonSerializer.h"

rapidjson::Value
CoordinatesJsonSerializer::toJson(Coordinates coordinates, rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value json;
    json.SetArray();
    rapidjson::Value lonVal;
    lonVal.SetDouble((double)coordinates.longitude());
    json.PushBack(lonVal, allocator);
    rapidjson::Value latVal;
    latVal.SetDouble((double)coordinates.latitude());
    json.PushBack(latVal, allocator);
    return json;
}