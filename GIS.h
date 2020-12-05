
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
#include "GISNamedTypes.h"
#include "Logger.h"

class EntityJsonParser;

class GIS
{
protected:
    std::unordered_map<EntityId, std::unique_ptr<Entity>> entities;
    std::shared_ptr<EntityJsonParser> entityJsonParser;
    EntityJsonSerializer entityJsonSerializer;
    JsonFileWriter jsonFileWriter;
    std::shared_ptr<Grid> grid;
    std::unique_ptr<TopologicalSearch> topologicalSearch;
    std::unique_ptr<Logger> logger;

public:
    GIS();

    std::size_t clear();

    std::vector<EntityId> loadMapFile(const std::string &filename);

    const Entity *getEntityById(const EntityId &id) const;

    std::size_t saveMapFile(const std::string &filename);

    /* returns vector of EntityId of all the entities in the map with the name search_name */
    std::vector<EntityId> getEntities(const std::string &search_name);

    /* returns vector of EntityId of all the entities in the map with the name search_name in the given
     * circle which is defined by the center Coordinates and the radius  */
    std::vector<EntityId> getEntities(const std::string &search_name, const Coordinates &center, Meters radius);

    /* returns the closest point on entity's perimeter from the given coordinates*/
    std::optional<Coordinates> getEntityClosestPoint(const EntityId &id, const Coordinates &coordinates);

    /* BFS-like algorithms which spreads to all direction in an even way level-by-level and searches for the closest way.
    * Once a way found, the spreading is stopped and the function returns the closest point on all
    * the ways on the current level */
    std::pair<Coordinates, EntityId> getWayClosestPoint(const Coordinates &coords);


private:

    /* loads all the entities inside rapidjson::Document */
protected:
    std::vector<EntityId> loadEntities(rapidjson::Document &document);

    /* returns vector with all the entities within the radius of the given coordinates */
    std::vector<const Entity *> getEntities(const Coordinates &coordinates, Meters radius);

    bool filterEntityByName(const Entity *entity, const std::string &search_name);

    bool addEntity(std::unique_ptr<Entity> entity);
};

#endif //EX1_GIS_H
