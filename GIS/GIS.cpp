#include <vector>
#include <optional>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "GIS.h"
#include <stdexcept>
#include <iostream>
#include <stack>
#include <set>
#include "../entities/JsonHandlers/parsers/EntityJsonParser.h"
#include "../Common/CoordinatesMath.h"
#include "../Utils.h"
#include <climits>

GIS::GIS() : entityJsonParser(std::make_shared<EntityJsonParser>()), grid(std::make_shared<Grid>()),
             topologicalSearch(std::make_unique<TopologicalSearch>()), logger(std::make_unique<Logger>()),
             ids(std::vector<EntityId>()) {
    logger->initialize();
}

std::size_t GIS::clear() {
    int size = entities.size();
    entities.clear();
    grid->clear();
    ids.clear();
    return size;
}

std::vector<EntityId> GIS::loadMapFile(const std::string &filename) {
    std::vector<EntityId> entityIds;
    FILE *fp = fopen(filename.c_str(), "rb"); // non-Windows use "r"
    if (fp == nullptr) {
        logger->error("Couldn't load map because the file '" + filename + "' could not be opened");
    } else {
        rapidjson::Document document;
        char readBuffer[65536];
        rapidjson::FileReadStream fileReadStream(fp, readBuffer, sizeof(readBuffer));
        document.ParseStream(fileReadStream);
        fclose(fp);
        if (document.HasParseError()) {
            logger->error("The JSON map is invalid");
        } else if (!document.IsArray()) {
            logger->error("The JSON map is not an array");
        } else {
            bool fileContainsIds = entityJsonParser->containsIds(document);
            entityJsonParser->setGenerateIds(fileContainsIds);
            entityIds = loadEntities(document);
        }
    }

    return entityIds;
}

std::size_t GIS::saveMapFile(const std::string &filename) {
    rapidjson::Document doc;
    doc.SetArray();
    std::size_t size = 0;
    for (EntityId id: ids) {
        rapidjson::Value entityVal = entities[id]->toJson(doc.GetAllocator());
        doc.PushBack(entityVal, doc.GetAllocator());
        size++;
    }
    jsonFileWriter.write(doc, filename);
    return size;
}

std::vector<EntityId> GIS::getEntities(const std::string &search_name) {
    std::vector<EntityId> entityIds;
    for (const auto &pair : entities) {
        if (filterEntityByName(pair.second.get(), search_name)) {
            entityIds.push_back(pair.first);
        }
    }
    return entityIds;
}

bool GIS::filterEntityByName(const Entity *entity, const std::string &search_name) {
    return entity->getName() == search_name;
}

std::vector<EntityId>
GIS::getEntities(const std::string &search_name, const Coordinates &coordinates, Meters radius) {
    std::vector<const Entity *> foundEntities = getEntities(coordinates, radius);
    std::vector<EntityId> ids;
    for (auto entity : foundEntities) {
        if (filterEntityByName(entity, search_name)) {
            ids.push_back(entity->getId());
        }
    }
    return ids;
}

std::optional<Coordinates> GIS::getEntityClosestPoint(const EntityId &entityId, const Coordinates &coordinates) {
    if (entities.find(entityId) == entities.end()) {
        return {};
    }
    return entities.find(entityId)->second->getGeometry()->getClosestPoint(coordinates);
}

std::pair<Coordinates, EntityId> GIS::getWayClosestPoint(const Coordinates &coord) const {
    return getWayClosestPoint(coord, Restrictions(""));
}

std::pair<Coordinates, EntityId> GIS::getWayClosestPoint(const Coordinates &coord, const Restrictions &res) const {
    bool wayFound = false;
    int level = 0;
    std::stack<Grid::GridCell> stack;
    std::unordered_set<Grid::GridCell> cellsVisited;
    std::unordered_set<EntityId> idsSeen;
    Coordinates closest(Longitude(0), Latitude(0));
    Meters shortestDistance(INT_MAX);
    EntityId closestEntityId("");
    std::stack<Grid::GridCell> nextStack;
    stack.push(grid->truncateCoordinates(coord));
    cellsVisited.insert(grid->truncateCoordinates(coord));

    while (!stack.empty() && level < max_closest_way_grid_levels) {
        while (!stack.empty()) {
            Grid::GridCell cell = stack.top();
            stack.pop();
            CellEntities cellEntities = grid->getEntitiesOnGrid(cell);
            for (auto &entityId : cellEntities.getEntities()) {
                if (idsSeen.find(entityId) != idsSeen.end()) {  // id was already seen
                    continue;
                }
                idsSeen.insert(entityId);
                const std::unique_ptr<Entity> &entity = entities.find(entityId)->second;
                if (!(entity->getType() == "Way")) { // not a way
                    continue;
                }
                auto &way = (const std::unique_ptr<Way> &) entity;
                Coordinates candidate = entity->getGeometry()->getClosestPoint(coord);
                Meters distance = CoordinatesMath::calculateDistance(candidate, coord);
                if (!isWayRestricted(*way, res, distance)) {
                    wayFound = true;
                    if (distance < shortestDistance) {
                        closest = candidate;
                        shortestDistance = distance;
                        closestEntityId = entityId;
                    }
                }
            }
            if (!wayFound) {
                std::vector<Grid::GridCell> neighbors = grid->getCellNeighbors(cell);
                // push to stack all the cells that were not visited yet.
                for (auto &neighbor : neighbors) {
                    if (cellsVisited.find(neighbor) == cellsVisited.end()) {
                        nextStack.push(neighbor);
                        cellsVisited.insert(neighbor);
                    }
                }
            } else if (stack.empty()) {
                return {closest, closestEntityId};
            }
        }
        std::swap(stack, nextStack);
        level++;
    }

    std::pair<Coordinates, EntityId> result = std::make_pair(closest, closestEntityId);
    if (!wayFound) {
        auto fallback = getWayClosestPointFallback(coord, res);
        if (fallback.has_value()) {
            result = fallback.value();
        }
    }
    return result;
}

bool GIS::isWayRestricted(const Way &way, const Restrictions &res, const Meters &distanceFromCoord) const {
    bool restricted = Utils::isWayRestricted(way, res);
    if (!restricted) {
        restricted = way.isHighway() && distanceFromCoord > max_distance_from_highway;
    }
    return restricted;
}

std::vector<EntityId> GIS::loadEntities(rapidjson::Document &document) {
    std::vector<EntityId> entityIds;
    for (auto &jsonEntity : document.GetArray()) {
        EntityId entityId("");
        try {
            std::unique_ptr<Entity> entity = entityJsonParser->parse(jsonEntity, *this);
            entityId = entity->getId();
            if (addEntity(std::move(entity))) {
                entityIds.push_back(entityId);
            }
        }
        catch (const std::runtime_error &e) {
            std::string err = "Couldn't load invalid entity";
            if (!((std::string) entityId).empty()) {
                err += " with id '" + (std::string) entityId + "'";
            }
            logger->error(err, e);
        }
    }
    return entityIds;
}

Entity *GIS::getEntityById(const EntityId &id) const {
    auto pair = entities.find(id);
    if (pair == entities.end()) {
        return nullptr;
    } else {
        return pair->second.get();
    }
}

std::vector<const Entity *> GIS::getEntities(const Coordinates &coordinates, Meters radius) {
    std::vector<Grid::GridCell> cells = topologicalSearch->searchCircleInGrid(*grid, coordinates, radius);
    std::set<EntityId> searchedEntityIds;
    std::vector<const Entity *> foundEntities;
    for (Grid::GridCell cell : cells) {
        CellEntities cellEntities = grid->getEntitiesOnGrid(cell);
        for (EntityId id : cellEntities.getEntities()) {
            if (searchedEntityIds.find(id) == searchedEntityIds.end()) {
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

bool GIS::addEntity(std::unique_ptr<Entity> entity) {
    bool success = true;
    EntityId entityId = entity->getId();
    // if entityId not loaded yet
    if (entities.find(entityId) == entities.end()) {
        grid->setEntityOnGrid(*entity);
        entities.emplace(entityId, std::move(entity));
        ids.push_back(entityId);
    } else {
        success = false;
        logger->error("Entity with id '" + (std::string) entityId + "' already exists");
    }
    return success;
}

const Way &GIS::getWay(const EntityId &id) const {
    if (entities.find(id) == entities.end()) {
        throw std::runtime_error("Id not found");
    }
    Entity *entity = entities.find(id)->second.get();
    if (entity->getType() != "Way") {
        throw std::runtime_error("Entity type is not a way");

    }
    return *(dynamic_cast<Way *>(entity));
}

std::vector<EntityId> GIS::getWaysByJunction(const EntityId &id) const {
    if (entities.find(id) == entities.end()) {
        return std::vector<EntityId>();
    }
    Entity *entity = entities.find(id)->second.get();
    if (entity->getType() != "Junction") {
        return std::vector<EntityId>();
    }
    return (dynamic_cast<Junction *>(entity))->getWays();
}

const Meters &GIS::getMaxDistanceFromHighway() const {
    return max_distance_from_highway;
}

std::optional<std::pair<Coordinates, EntityId>>
GIS::getWayClosestPointFallback(const Coordinates &coord, const Restrictions &res) const {
    std::optional<std::pair<Coordinates, EntityId>> foundWay;
    Meters shortestDistance(0);
    for (auto &entityPair : entities) {
        Entity &entity = *entityPair.second;
        if (entity.getType() == "Way") {
            Way &way = (Way &) entity;
            Coordinates closestPoint = way.getGeometry()->getClosestPoint(coord);
            Meters distance = CoordinatesMath::calculateDistance(coord, closestPoint);
            if (!isWayRestricted(way, res, distance) && (!foundWay.has_value() || distance < shortestDistance)) {
                foundWay = std::make_pair(closestPoint, way.getId());
                shortestDistance = distance;
            }
        }
    }
    return foundWay;
}

