#include "EntityJsonParser.h"

Entity *EntityJsonParser::parse(rapidjson::Value &doc) {
    std::string type = doc["type"].GetString();
    if (type == "POI") {
        return parsePoi(doc);
    } else {
        throw std::runtime_error("Entity type not supported");
    }
}

POI *EntityJsonParser::parsePoi(rapidjson::Value &doc) {
    EntityId entityId = getEntityId(doc);
    std::string name = getName(doc);
    std::string description = getDescription(doc);

    std::vector<std::string> categoryTags = getCategoryTags(doc);
    std::vector<std::string> accessibility = getAccessibility(doc);
    if (!doc.HasMember("geometry") || !doc["geometry"].IsObject()) {
        throw std::runtime_error("JSON entity doesn't contain geometry");
    }
    Geometry geometry = geometryJsonParser.parseGeometry(doc["geometry"]);

    POI *poi = new POI(entityId, name, description, categoryTags, accessibility, geometry);
    return poi;
}

EntityId EntityJsonParser::getEntityId(rapidjson::Value &doc) {
    if (!doc.HasMember("id") || !doc["id"].IsString()) {
        throw std::runtime_error("JSON entity doesn't contain id");
        //TODO: instead, generate unique id
    } else {
        return EntityId(doc["id"].GetString());
    }
}

std::string EntityJsonParser::getName(rapidjson::Value &doc) {
    if (!doc.HasMember("name") || !doc["id"].IsString()) {
        throw std::runtime_error("JSON entity doesn't contain name");
    }
    return doc["name"].GetString();
}

std::string EntityJsonParser::getDescription(rapidjson::Value &doc) {
    //optional entry
    std::string description;
    if (doc.HasMember("description") && doc["description"].IsString()) {
        description = doc["description"].GetString();
    }
    return description;
}

std::vector<std::string> EntityJsonParser::getCategoryTags(rapidjson::Value &doc) {
    //optional entry
    std::vector<std::string> categoryTags;

    if (doc.HasMember("category_tags") && doc["category_tags"].IsArray()) {
        for (auto &category : doc["category_tags"].GetArray()) {
            if (category.IsString()) {
                categoryTags.push_back(category.GetString());
            }
        }
    }

    return categoryTags;
}

std::vector<std::string> EntityJsonParser::getAccessibility(rapidjson::Value &doc) {
    //optional entry
    std::vector<std::string> accessibility;

    if (doc.HasMember("accessibility") && doc["accessibility"].IsArray()) {
        for (auto &acc : doc["accessibility"].GetArray()) {
            if (acc.IsString()) {
                accessibility.push_back(acc.GetString());
            }
        }
    }

    return accessibility;
}

