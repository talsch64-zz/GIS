
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

class EntityJsonParser;

class GIS {
    std::unordered_map<EntityId, Entity *> entities;
    EntityJsonParser *entityJsonParser;
    EntityJsonSerializer entityJsonSerializer;
    JsonFileWriter jsonFileWriter;
    std::shared_ptr<Grid> grid;
    TopologicalSearch topologicalSearch;
    IdGenerator idGenerator;

public:
    GIS();

    ~GIS();

    std::size_t clear();

    std::vector<EntityId> loadMapFile(const std::string &filename);

    const std::unordered_map<EntityId, Entity *> &getEntitiesMap() const;

    std::size_t saveMapFile(const std::string &filename);

    std::vector<EntityId> getEntities(const std::string &search_name);

    std::vector<EntityId> getEntities(const std::string &search_name, const Coordinates &, Meters radius);

    std::optional<Coordinates> getEntityClosestPoint(const EntityId &, const Coordinates &);

    std::pair<Coordinates, EntityId> getWayClosestPoint(const Coordinates &);


private:
    void loadEntities(rapidjson::Document &document, std::vector<EntityId> &entityIds, bool generateId, bool loadWays,
                      bool loadNoneWays);


};

#endif //EX1_GIS_H
