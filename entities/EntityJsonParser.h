#ifndef EX1_ENTITYJSONPARSER_H
#define EX1_ENTITYJSONPARSER_H

#include <rapidjson/document.h>
#include "POI.h"
#include "Junction.h"
#include "geometry/GeometryJsonParser.h"
#include "Way.h"
#include "../IdGenerator.h"
#include<memory>

class GIS;

class EntityJsonParser {
    GeometryJsonParser geometryJsonParser;
    CoordinatesJsonParser coordinatesJsonParser;
    bool _generateIds = false;
    IdGenerator idGenerator;


public:
    std::unique_ptr<Entity> parse(rapidjson::Value &doc, GIS &gis);

    bool containsIds (rapidjson:: Value &doc);

    void setGenerateIds(bool fileContainsId);

    bool generateIds() const;

    bool isWay(rapidjson::Value &jsonEntity);

private:

    std::unique_ptr<POI> parsePoi(rapidjson::Value &doc);

    std::unique_ptr<Junction> parseJunction(rapidjson::Value &doc);

    EntityId parseEntityId(rapidjson::Value &doc);

    std::string parseName(rapidjson::Value &doc);

    std::string parseDescription(rapidjson::Value &doc);

    std::vector<std::string> parseCategoryTags(rapidjson::Value &doc);

    std::vector<std::string> parseAccessibility(rapidjson::Value &doc);

    std::unique_ptr<Way> parseWay(rapidjson::Value &doc, const GIS &gis);

    std::string parseDirection(rapidjson::Value &doc);

    int parseSpeedLimit(rapidjson::Value &doc);

    bool parseTollRoad(rapidjson::Value &doc);

    std::vector<std::string> parseRestricted(rapidjson::Value &doc);

    EntityId parseJunctionId(rapidjson::Value &doc, const char *direction);

//    std::vector<Coordinates> parseCurves(rapidjson::Value &doc);

//    std::vector<Coordinates> parseCoordinates(rapidjson::Value &doc);

};

#endif //EX1_ENTITYJSONPARSER_H
