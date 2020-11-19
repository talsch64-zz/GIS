#include <iostream>
#include "EntityJsonSerializer.h"
#include "../../Entity.h"
#include "../../POI.h"
#include "../../Junction.h"
#include "../../Way.h"

rapidjson::Value EntityJsonSerializer::entityToJson(Entity *entity, rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value json;
    json.SetObject();
    rapidjson::Value id;
    std::string idStr = entity->getId();
    id.SetString(idStr.c_str(), idStr.length(), allocator);
    json.AddMember("id", id, allocator);

    rapidjson::Value name;
    name.SetString(entity->getName().c_str(), entity->getName().length(), allocator);
    json.AddMember("name", name, allocator);

    if (entity->getDescription()) {
        rapidjson::Value description;
        description.SetString(entity->getDescription().value().c_str(), entity->getDescription().value().length(),
                              allocator);
        json.AddMember("description", description, allocator);
    }

    rapidjson::Value categoryTags;
    categoryTags.SetArray();
    for (std::string tag : entity->getCategoryTags()) {
        rapidjson::Value tagVal;
        tagVal.SetString(tag.c_str(), tag.length(), allocator);
        categoryTags.PushBack(tagVal, allocator);
    }
    json.AddMember("category_tags", categoryTags, allocator);

    return json;
}

rapidjson::Value EntityJsonSerializer::toJson(POI *entity, rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value json = entityToJson(entity, allocator);

    rapidjson::Value accessibility;
    accessibility.SetArray();
    for (std::string acc : entity->getAccessibility()) {
        rapidjson::Value accVal;
        accVal.SetString(rapidjson::StringRef(acc.c_str()));
        accessibility.PushBack(accVal, allocator);
    }
    json.AddMember("accessibility", accessibility, allocator);
    json = setGeometry(std::move(json), entity->getGeometry().get(),
                       allocator);

    return json;
}

rapidjson::Value EntityJsonSerializer::setGeometry(rapidjson::Value entityJson, Geometry *geometry,
                                                   rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value geometryJson = geometry->toJson(allocator);
    entityJson.AddMember("geometry", geometryJson, allocator);
    return entityJson;
}

rapidjson::Value EntityJsonSerializer::toJson(Junction *entity, rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value json = entityToJson(entity, allocator);
    rapidjson::Value coord = coordinatesJsonSerializer.toJson(entity->getCoordinates(), allocator);
    json.AddMember("coordinates", coord, allocator);
    return json;
}

rapidjson::Value
EntityJsonSerializer::toJson(Way *entity, rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value json = entityToJson(entity, allocator);
    rapidjson::Value direction;
    direction.SetString(entity->getDirection().c_str(), entity->getDirection().length(), allocator);
    json.AddMember("direction", direction, allocator);

    rapidjson::Value speedLimit;
    speedLimit.SetInt(entity->getSpeedLimit());
    json.AddMember("speed_limit", speedLimit, allocator);

    rapidjson::Value tollRoad;
    tollRoad.SetBool(entity->isTollRoad());
    json.AddMember("toll_road", tollRoad, allocator);

    rapidjson::Value from;
    std::string fromStr = entity->getFrom();
    from.SetString(fromStr.c_str(), fromStr.length(), allocator);
    json.AddMember("from", from, allocator);

    rapidjson::Value to;
    std::string toStr = entity->getTo();
    to.SetString(toStr.c_str(), toStr.length(), allocator);
    json.AddMember("to", to, allocator);

    rapidjson::Value restricted;
    restricted.SetArray();
    for (const std::string &restrictedStr : entity->getRestricted()) {
        rapidjson::Value restrictedVal;
        restrictedVal.SetString(restrictedStr.c_str(), restrictedStr.length(), allocator);
        restricted.PushBack(restrictedVal, allocator);
    }
    json.AddMember("restricted", restricted, allocator);

    rapidjson::Value curves;
    curves.SetArray();
    for (Coordinates coord : entity->getCurves()) {
        rapidjson::Value coordVal = coordinatesJsonSerializer.toJson(coord, allocator);
        curves.PushBack(coordVal, allocator);
    }
    json.AddMember("curves", curves, allocator);

    return json;
}
