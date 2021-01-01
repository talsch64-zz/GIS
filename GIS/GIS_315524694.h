
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
#include "../IdGenerator.h"
#include "../Common/GISNamedTypes.h"
#include "../Logger.h"
#include "../Common/Restrictions.h"
#include "../Common/AbstractGIS.h"

class EntityJsonParser;


/// GIS application that allows the user to create interactive queries, store spatial data (Entities), and import the data into JSON files.
/// Operations supported:
/// * Load entities from JSON files
/// * Save entities into JSON files
/// * Clear all data currently stored
/// * Search entities by ids, names and radius from a given Coordinates
/// * Search closest point on entities perimeters from a given Coordinates
/// * Search closest way (and the closest Coordinates on the way) from a given Coordinates


class GIS_315524694: public AbstractGIS {
protected:
    /**
     * Max distance to be from a highway for it to be a valid start of a route
     */
    static constexpr Meters max_distance_from_highway = Meters(3);
    /**
     * Max depth of BFS to search the grid for closest way before resorting to a fallback (iterating over all the ways)
     */
    const int max_closest_way_grid_levels = 250;
    std::unordered_map<EntityId, std::unique_ptr<Entity>> entities;
public:
    static const Meters &getMaxDistanceFromHighway();

protected:
    std::shared_ptr<EntityJsonParser> entityJsonParser;
    EntityJsonSerializer entityJsonSerializer;
    JsonFileWriter jsonFileWriter;
    std::shared_ptr<Grid> grid;
    std::unique_ptr<TopologicalSearch> topologicalSearch;
    std::unique_ptr<Logger> logger;
    std::vector<EntityId> ids;

public:
    GIS_315524694();

    std::size_t clear();

    std::vector<EntityId> loadMapFile(const std::string &filename) override;

    Entity *getEntityById(const EntityId &id) const;

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
    std::tuple<Coordinates, EntityId, std::size_t> getWayClosestPoint(const Coordinates &coord) const override;

    /**
     * Find the closest way to a given coordinate, with restrictions
     * @param coords Coordinate to search
     * @param res Restrictions for the ways
     * @return A pair of the closest point on the way and the entity id of the way
     */
    std::tuple<Coordinates, EntityId, std::size_t> getWayClosestPoint(const Coordinates &coord, const Restrictions &res) const override;

    /**
 * @brief Get the Way object et EntityId of a Way and return the Way itself.
 *        In case the id does not represent a valid way throw an exception of your choice.
 *
 * @param wayId - ID of Way
 * @return const Way&
 */
    const AbstractWay &getWay(const EntityId &wayId) const override;

    /**
	 * @brief The function shall get EntityId of a Junction and return a vector of Ids of all the ways that start at this junction and those which end at this junction and are bidirectional.
	 *        If the given Id is not known or is not of a junction the function shall return an empty vector (you may log an error).
	 *
	 * @param junctionId
	 * @return std::vector<EntityId> - A vector of Ids of all the Ways that start at given junction (also bidirectional Ways which end at this junction).
	 */
    std::vector<EntityId> getWaysByJunction(const EntityId &junctionId) const override;

protected:
    /* loads all the entities inside rapidjson::Document */
    std::vector<EntityId> loadEntities(rapidjson::Document &document);

    /* returns vector with all the entities within the radius of the given coordinates */
    std::vector<const Entity *> getEntities(const Coordinates &coordinates, Meters radius);

    bool filterEntityByName(const Entity *entity, const std::string &search_name);

    bool addEntity(std::unique_ptr<Entity> entity);

    /**
     * Is way restricted (aka invalid) as a closest way to a coordinate
     * @param way Way object
     * @param res Restrictions on the way
     * @param distanceFromCoord distance from the searched coordinate
     * @return
     */
    bool isWayRestricted(const Way &way, const Restrictions &res, const Meters &distanceFromCoord) const;

    /**
     * If BFS search up to a maximum depth didn't find a closest way on the grid,
     * a fallback is called which iterates over all the ways.
     * @param coord Search way closest to this coordinate
     * @param res Restrictions on the ways search
     * @return A pair of the closest point on the way and the way's id. If no way was found, an empty optional is returned.
     */
    std::optional<std::tuple<Coordinates, EntityId, std::size_t>> getWayClosestPointFallback(const Coordinates &coord, const Restrictions &res) const;
};

#endif //EX1_GIS_H
