#include <memory>
#include "EntityJsonParser.h"
#include "Way.h"

std::unique_ptr<Entity> EntityJsonParser::parse(rapidjson::Value &doc) {
    std::string type = doc["type"].GetString();
    if (type == "POI") {
        return parsePoi(doc);
    }
    if (type == "Junction") {
        return parseJunction(doc);
    }
    if (type == "Way") {
        return parseWay(doc);
    } else {
        throw std::runtime_error("Entity type not supported");
    }
}

bool EntityJsonParser::isWay(rapidjson::Value &jsonEntity) {
    std::string type = jsonEntity["type"].GetString();
    return (type == "Way");
}

std::unique_ptr<Way> EntityJsonParser::parseWay(rapidjson::Value &doc) {
    std::string id = parseEntityId(doc);
    std::string name = parseName(doc);
    std::string description = parseDescription(doc);
    std::vector<std::string> categoryTags = parseCategoryTags(doc);
    std::string direction = parseDirection(doc);
    int speedLimit = parseSpeedLimit(doc);
    bool tollRoad = parseTollRoad(doc);
    std::vector<std::string> restricted = parseRestricted(doc);
    std::string from = parseJunctionId(doc, "from");
    std::string to = parseJunctionId(doc, "to");
    std::unique_ptr<Geometry> geometry = geometryJsonParser.parseWayGeometry(doc);
    std::unique_ptr<Way> way(new Way(id, name, description, categoryTags, std::move(geometry), from, to, direction, speedLimit, tollRoad, restricted));
    return way;
}

std::unique_ptr<Junction> EntityJsonParser::parseJunction(rapidjson::Value &doc) {
    std::string id = parseEntityId(doc);
    std::string name = parseName(doc);
    std::string description = parseDescription(doc);
    std::vector<std::string> categoryTags = parseCategoryTags(doc);
    std::unique_ptr<Geometry> geometry = geometryJsonParser.parseJunctionGeometry(doc);
    std::unique_ptr<Junction> junction(new Junction(id, name, description, categoryTags, std::move(geometry)));
    return junction;
}


std::unique_ptr<POI> EntityJsonParser::parsePoi(rapidjson::Value &doc) {
    std::string id = parseEntityId(doc);
    std::string name = parseName(doc);
    std::string description = parseDescription(doc);
    std::vector<std::string> categoryTags = parseCategoryTags(doc);
    std::vector<std::string> accessibility = parseAccessibility(doc);
    std::unique_ptr<Geometry> geometry = geometryJsonParser.parsePOIGeometry(doc);
    std::unique_ptr<POI> poi(new POI(id, name, description, categoryTags, accessibility, std::move(geometry)));
    return poi;
}

std::string EntityJsonParser::parseEntityId(rapidjson::Value &doc) {
    if (toParseId()) {
        if (!doc.HasMember("id") || !doc["id"].IsString() || doc["id"].GetString() == "") {
            throw std::runtime_error("JSON entity doesn't contain id");
        } else {
            return doc["id"].GetString();
        }
    }
    return "";
}

std::string EntityJsonParser::parseName(rapidjson::Value &doc) {
    if (!doc.HasMember("name") || !doc["id"].IsString()) {
        throw std::runtime_error("JSON entity doesn't contain name");
    }
    return doc["name"].GetString();
}

std::string EntityJsonParser::parseDescription(rapidjson::Value &doc) {
    //optional entry
    std::string description;
    if (doc.HasMember("description") && doc["description"].IsString()) {
        description = doc["description"].GetString();
    }
    return description;
}

std::vector<std::string> EntityJsonParser::parseCategoryTags(rapidjson::Value &doc) {
    //optional entry
    std::vector<std::string> categoryTags;

    if (doc.HasMember("category_tags") && doc["category_tags"].IsArray()) {
        for (auto &category : doc["category_tags"].GetArray()) {
            if (category.IsString()) {
                categoryTags.emplace_back(category.GetString());
            }
        }
    }

    return categoryTags;
}

std::vector<std::string> EntityJsonParser::parseAccessibility(rapidjson::Value &doc) {
    //optional entry
    std::vector<std::string> accessibility;

    if (doc.HasMember("accessibility") && doc["accessibility"].IsArray()) {
        for (auto &acc : doc["accessibility"].GetArray()) {
            if (acc.IsString()) {
                accessibility.emplace_back(acc.GetString());
            }
        }
    }

    return accessibility;
}

std::string EntityJsonParser::parseDirection(rapidjson::Value &doc) {
    std::string direction;
    if (doc.HasMember("direction") && doc["direction"].IsString()) {
        direction = doc["direction"].GetString();
    }
    return direction;
}

int EntityJsonParser::parseSpeedLimit(rapidjson::Value &doc) {
    if (doc.HasMember("speed_limit") && doc["speed_limit"].IsNumber()) {
        return doc["speed_limit"].GetInt();
    }
    return 0;
}

bool EntityJsonParser::parseTollRoad(rapidjson::Value &doc) {
    if (doc.HasMember("toll_road") && doc["toll_road"].IsBool()) {
        return doc["toll_road"].GetBool();
    };
    return false;
}

std::vector<std::string> EntityJsonParser::parseRestricted(rapidjson::Value &doc) {
    //optional entry
    std::vector<std::string> restricted;
    if (doc.HasMember("restricted") && doc["restricted"].IsArray()) {
        for (auto &label : doc["restricted"].GetArray()) {
            if (label.IsString()) {
                restricted.emplace_back(label.GetString());
            }
        }
    }
    return restricted;
}

std::string EntityJsonParser::parseJunctionId(rapidjson::Value &doc, const char *direction) {
    std::string junctionId;
    if (doc.HasMember(direction) && doc[direction].IsString()) {
        junctionId = doc[direction].GetString();
    }
    return junctionId;
}

bool EntityJsonParser::containsIds(rapidjson::Value &doc) {
    bool hasId = false;
    for (auto &jsonEntity : doc.GetArray()) {
        if (jsonEntity.HasMember("id") && jsonEntity["id"].IsString() && (strcmp(jsonEntity["id"].GetString(), "")) != 0) {
            hasId = true;
            break;
        }
    }
    return hasId;
}

bool EntityJsonParser::toParseId() const {
    return parseId;
}

void EntityJsonParser::setParseId(bool parseId) {
    EntityJsonParser::parseId = parseId;
}


//std::vector<Coordinates> EntityJsonParser::parseCurves(rapidjson::Value &doc) {
//    //optional entry
//    std::vector<Coordinates> curves;
//    if (doc.HasMember("curves") && doc["curves"].IsArray()) {
//        for (auto &coordinates : doc["curves"].GetArray()) {
//            curves.push_back(coordinatesJsonParser.parse(coordinates));
//        }
//    }
//    return curves;
//}