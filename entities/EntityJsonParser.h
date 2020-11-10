#ifndef EX1_ENTITYJSONPARSER_H
#define EX1_ENTITYJSONPARSER_H

#include <rapidjson/document.h>
#include "POI.h"
#include "geometry/GeometryJsonParser.h"

class EntityJsonParser {
    GeometryJsonParser geometryJsonParser;

public:
    Entity parse(rapidjson::Value &doc);

private:
    POI parsePoi(rapidjson::Value &doc);

    EntityId getEntityId(rapidjson::Value &doc);

    std::string getName(rapidjson::Value &doc);

    std::string getDescription(rapidjson::Value &doc);

    std::vector<std::string> getCategoryTags(rapidjson::Value &doc);

    std::vector<std::string> getAccessibility(rapidjson::Value &doc);
};

#endif //EX1_ENTITYJSONPARSER_H
