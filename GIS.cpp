

#include <vector>
#include <optional>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "GIS.h"
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

    for (auto &jsonEntity : document.GetArray()) {
        try {
            std::unique_ptr<Entity> entity = entityJsonParser.parse(jsonEntity);
            EntityId entityId = entity->getId();
            entities.emplace(entityId, std::move(entity));
            entityIds.push_back(entityId);

        }
        catch (const std::runtime_error &e) {
        }
    }
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
