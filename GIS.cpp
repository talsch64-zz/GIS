

#include <vector>
#include <optional>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "GIS.h"
#include "entities/geometry/PointList.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <set>
#include "entities/EntityJsonParser.h"


GIS::GIS() : grid(std::make_shared<Grid>()), entityJsonParser(new EntityJsonParser()),
             topologicalSearch(std::make_unique<TopologicalSearch>()) {
}

GIS::~GIS() { delete entityJsonParser; }

std::size_t GIS::clear() {
    return 0;
}

std::vector<EntityId> GIS::loadMapFile(const std::string &filename) {
    rapidjson::Document document;
    FILE *fp = fopen(filename.c_str(), "rb"); // non-Windows use "r"
//    TODO print to log if file could not be open
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
    bool fileContainsIds = entityJsonParser->containsIds(document);
    entityJsonParser->setGenerateIds(fileContainsIds);
    std::vector<EntityId> entityIds = loadEntities(document);

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
GIS::getEntities(const std::string &search_name, const Coordinates &coordinates, Meters radius) {
    //TODO: filter also by name
    std::vector<const Entity *> foundEntities = getEntities(coordinates, radius);
    std::vector<EntityId> ids;
    for (auto entity : foundEntities) {
        ids.push_back(entity->getId());
    }
    return ids;
}

std::optional<Coordinates> GIS::getEntityClosestPoint(const EntityId &, const Coordinates &) {
    Coordinates coord(Longitude(0), Latitude(0));
    return coord;
}

std::pair<Coordinates, EntityId> GIS::getWayClosestPoint(const Coordinates &) {
    Coordinates coord(Longitude(0), Latitude(0));
    std::pair<Coordinates, EntityId> p(coord, "something");
    return p;
}


std::vector<EntityId> GIS::loadEntities(rapidjson::Document &document) {
    std::vector<EntityId> entityIds;
    for (auto &jsonEntity : document.GetArray()) {
        try {
            std::unique_ptr<Entity> entity = entityJsonParser->parse(jsonEntity, *this);
            EntityId entityId = entity->getId();
            // if entityId not loaded yet
            if (entities.find(entityId) == entities.end()) {
                grid->setEntityOnGrid(*entity);
                entities.emplace(entityId, std::move(entity));
                entityIds.push_back(entityId);

            } else {
//                TODO print to log that id is not unique
            }
        }
        catch (const std::runtime_error &e) {
//            TODO print to log error that entity is invalid
        }
    }
    return entityIds;
}

const Entity *GIS::getEntityById(const EntityId &id) const {
    auto pair = entities.find(id);
    if (pair == entities.end()) {
        return nullptr;
    } else {
        return pair->second.get();
    }
}

std::vector<const Entity *> GIS::getEntities(const Coordinates &coordinates, Meters radius) {
    std::vector<Grid::GridCell> cells = topologicalSearch->searchCircleInGrid(*grid.get(), coordinates, radius);
    std::set<EntityId> searchedEntityIds;
    std::vector<const Entity *> foundEntities;
    for (Grid::GridCell cell : cells) {
        CellEntities cellEntities = grid->getEntitiesOnGrid(cell);
        for (EntityId id : cellEntities.getEntities()) {
            if (!searchedEntityIds.contains(id)) {
                searchedEntityIds.insert(id);
                const Entity *entity = getEntityById(id);
                if (entity->getGeometry()->isInCircle(topologicalSearch.get(), coordinates, radius)) {
                    foundEntities.push_back(entity);
                }
            }
        }
    }
    return foundEntities;
}
