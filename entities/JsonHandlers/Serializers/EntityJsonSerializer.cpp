#include <iostream>
#include "EntityJsonSerializer.h"
#include "../../Entity.h"
#include "../../POI.h"

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

    rapidjson::Value description;
    description.SetString(entity->getDescription().c_str(), entity->getDescription().length(), allocator);
    json.AddMember("description", description, allocator);

    rapidjson::Value categoryTags;
    categoryTags.SetArray();
    for (std::string tag : entity->getCategoryTags()) {
        rapidjson::Value tagVal;
        tagVal.SetString(tag.c_str(), tag.length(), allocator);
        categoryTags.PushBack(tagVal, allocator);
    }
    json.AddMember("categoryTags", categoryTags, allocator);

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