

#include <vector>
#include <optional>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "GIS.h"
#include "entities/geometry/PointList.h"
#include <stdexcept>
#include <iostream>
#include <fstream>

GIS::GIS() : grid(std::make_shared<Grid>()), topologicalSearch(grid) {
}

std::size_t GIS::clear() {
    return 0;
}

std::vector<EntityId> GIS::loadMapFile(const std::string &filename) {
    rapidjson::Document document;
    std::vector<EntityId> entityIds;
    FILE *fp = fopen(filename.c_str(), "rb"); // non-Windows use "r"

    char readBuffer[65536];
    rapidjson::FileReadStream fileReadStream(fp, readBuffer, sizeof(readBuffer));

    document.ParseStream(fileReadStream);

//    if (!ok) {
//        throw std::runtime_error("JSON parse error");
//    }
    if (!document.IsArray()) {
        //TODO: handle errors
        throw std::runtime_error("Map is not an array");
    }
    bool fileContainsIds = entityJsonParser.containsIds(document);
    entityJsonParser.setGenerateIds(fileContainsIds);

    loadNoneWaysEntities(document, entityIds, fileContainsIds);
    loadWaysEntities(document, entityIds, fileContainsIds);

    return entityIds;
}


std::size_t GIS::saveMapFile(const std::string &filename) {
    rapidjson::Document doc;
    doc.SetArray();
    std::size_t size = 0;
    for (auto &entityPair : entities) {
        rapidjson::Value entityVal = entityPair.second->toJson(doc.GetAllocator());
        doc.PushBack(entityVal, doc.GetAllocator());
        size++;
    }
    jsonFileWriter.write(doc, filename);
    return size;
}

std::vector<EntityId> GIS::getEntities(const std::string &search_name) { return std::vector<EntityId>(); }

std::vector<EntityId>
GIS::getEntities(const std::string &search_name, const Coordinates &, Meters radius) { return std::vector<EntityId>(); }

std::optional<Coordinates> GIS::getEntityClosestPoint(const EntityId &, const Coordinates &) {
    Coordinates coord(Longitude(0), Latitude(0));
    return coord;
}

std::pair<Coordinates, EntityId> GIS::getWayClosestPoint(const Coordinates &) {
    Coordinates coord(Longitude(0), Latitude(0));
    std::pair<Coordinates, EntityId> p(coord, "something");
    return p;
}


void GIS::loadNoneWaysEntities(rapidjson::Document &document, std::vector<EntityId> &entityIds, bool generateId) {
    for (auto &jsonEntity : document.GetArray()) {
        try {
            if (entityJsonParser.isWay(jsonEntity)) {
                // Way entities will be added in the end of the parsing
                continue;
            }
            std::unique_ptr<Entity> entity = entityJsonParser.parse(jsonEntity);
            //TODO parser should generate the id
            if (!generateId) {
                entity->setId(idGenerator.generateId());
            }

            EntityId entityId = entity->getId();
            // if entityId not loaded yet
            if (entities.find(entityId) == entities.end()) {
                entities.emplace(entityId, std::move(entity));
                entityIds.push_back(entityId);
            }
            else {
//                TODO print to log that id is not unique
            }
        }
        catch (const std::runtime_error &e) {
        }
    }
}

void GIS::loadWaysEntities(rapidjson::Document &document, std::vector<EntityId> &entityIds, bool generateId) {
    for (auto &jsonEntity : document.GetArray()) {
        try {
            if (!entityJsonParser.isWay(jsonEntity)) {
                continue;
            }
            std::unique_ptr<Entity> entity = entityJsonParser.parse(jsonEntity);
            if (!generateId) {
                entity->setId(idGenerator.generateId());
            }

            EntityId entityId = entity->getId();
            // if entityId not loaded yet
            if (entities.find(entityId) == entities.end()) {
                entities.emplace(entityId, std::move(entity));
                entityIds.push_back(entityId);
            }
            else {
//                TODO print to log that id is not unique
            }
        }
        catch (const std::runtime_error &e) {
        }
    }
}