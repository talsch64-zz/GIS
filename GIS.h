
#ifndef EX1_GIS_H
#define EX1_GIS_H

#include <utility>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <memory>
#include "entities/JsonHandlers/Serializers/EntityJsonSerializer.h"
#include "entities/JsonHandlers/Serializers/JsonFileWriter.h"
#include "search/Grid.h"
#include "search/TopologicalSearch.h"
#include "IdGenerator.h"
#include "GISdefs.h"
#include "Logger.h"

class EntityJsonParser;

class GIS {
    std::unordered_map<EntityId, std::unique_ptr<Entity>> entities;
    std::shared_ptr<EntityJsonParser> entityJsonParser;
    std::shared_ptr<Grid> grid;
    EntityJsonSerializer entityJsonSerializer;
    JsonFileWriter jsonFileWriter;
    std::unique_ptr<TopologicalSearch> topologicalSearch;
    std::unique_ptr<Logger> logger;

public:
    GIS();

    std::size_t clear();

    std::vector<EntityId> loadMapFile(const std::string &filename);

    const Entity *getEntityById(const EntityId &id) const;

    std::size_t saveMapFile(const std::string &filename);

    std::vector<EntityId> getEntities(const std::string &search_name);

    std::vector<EntityId> getEntities(const std::string &search_name, const Coordinates &, Meters radius);

    std::optional<Coordinates> getEntityClosestPoint(const EntityId &, const Coordinates &);

    /* BFS-like algorithms which spreads to all direction in an even way level-by-level and searches for the closest way.
     * Once a way found, the spreading is stopped and the function returns the closest point on all
     * the ways on the current level */
    std::pair<Coordinates, EntityId> getWayClosestPoint(const Coordinates &coords);


private:
    /* Loads all the entities inside rapidjson::Document */
    std::vector<EntityId> loadEntities(rapidjson::Document &document);


    std::vector<const Entity *> getEntities(const Coordinates &coordinates, Meters radius);

    bool filterEntityByName(const Entity *entity, const std::string &search_name);
};

#endif //EX1_GIS_H
